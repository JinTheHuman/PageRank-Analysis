// COMP2521 Assignment 2

// Written by: Jin Sunwoo z5363489
// Date: 19/11/2020

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "List.h"

#define MAX_URL_LENGTH 100

Graph setUpGraph(char filename[]);
void setUpGraphEdges(Graph g);
void appendTxt(char filename[], char urlName[]);
void pageRank(Graph g, double d, double diffPr, int maxIterations, double *pageRanks);
double wOut(Graph g, int index1, int index2);
int numOutlinks(Graph g, int index);
int numInlinks(Graph g, int index);
double wIn(Graph g, int index1, int index2);
double calculateDiff(double prevPageRanks[], double pageRanks[], int nV);
void setPrevPageRanks(double prevPageRanks[], double pageRanks[], int nV);
void writeToFileOrdered(Graph g, double *pageRanks);

int main(int argc, char *argv[]) {
    Graph Web = setUpGraph("collection.txt");

    double *pageRanks = malloc(sizeof(double) * getNumVertices(Web));
    pageRank(Web, atof(argv[1]), atof(argv[2]), atoi(argv[3]), pageRanks);\

    writeToFileOrdered(Web, pageRanks);
    
    free(pageRanks);
    freeGraph(Web);
    return 0;
}

// Initialises graph with urls in collection.txt
Graph setUpGraph(char filename[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(1);
    }
    
    int numUrls = 0;
    char name[MAX_URL_LENGTH];
    List Urls = ListNew();
    // Reads from file and adds url names and index to list
    while (fscanf(file, "%s", name) != EOF) {
        ListAppend(Urls, name, numUrls);
        numUrls++;
    }
    fclose(file);

    Graph Web = newGraph(numUrls);
    assignList(Web, Urls);
    
    setUpGraphEdges(Web);
    return Web;
}

// Iterates through urls and adds edges according to their text files
void setUpGraphEdges(Graph g) {
    ListIterator it = ListItNew(getGraphUrls(g));
    Edge edge;
    while (ListItHasNext(it)) {
        Node curr = ListItNext(it);
        edge.v = getUrlIndex(curr);
        
        char filename[MAX_URL_LENGTH + 4];
        // Adds .txt to end of urlname
        appendTxt(filename, getUrlName(curr));
        
        FILE *file = fopen(filename, "r");
        char outUrlname[MAX_URL_LENGTH];
        // Skips over the first line which by default is #start Section-1
        fscanf(file, "%[^\n]", outUrlname);
        
        // Scans in urls from file untill #end
        fscanf(file, "%s", outUrlname);
        while (strcmp(outUrlname, "#end") != 0) {
            // Finds the index of the url with the scanned in name
            edge.w = findUrlIndex(g, outUrlname);
            insertEdge(g, edge);
            fscanf(file, "%s", outUrlname);
        }

        fclose(file);
    }
    ListItFree(it);
}

// Adds .txt to the end of the url name and assigns it to filename
void appendTxt(char filename[], char urlName[]) {
    strcpy(filename, urlName);
    strcat(filename, ".txt");
}

// Writes to a file the urls in pagerank order
void writeToFileOrdered(Graph g, double *pageRanks) {
    FILE *file = fopen("pagerankList.txt", "w");
    int *visited = calloc(getNumVertices(g), sizeof(int));

    // Does selection sort by writing the next largest url to the file
    for (int i = 0; i < getNumVertices(g); i++) {
        ListIterator it = ListItNew(getGraphUrls(g));
        
        Node heaviestUrl = ListItNext(it);
        // Ensures that the initial heaviest url is not already written to file
        while (visited[getUrlIndex(heaviestUrl)] == 1) {
            heaviestUrl = ListItNext(it);
        }

        // Finds next largest url by pagerank in graph
        while (ListItHasNext(it)) {
            Node curr = ListItNext(it);
            
            int heavyIndex = getUrlIndex(heaviestUrl);
            int currIndex = getUrlIndex(curr);
            // Checks if curr is larger and has not already been written to file
            if (pageRanks[currIndex] > pageRanks[heavyIndex] && visited[currIndex] == 0) {
                heaviestUrl = curr;
            } 
            // Checks if the pageranks are equal
            else if (pageRanks[currIndex] == pageRanks[heavyIndex] && visited[currIndex] == 0) {
                // Checks whether curr comes first alphabetically
                if (strcmp(getUrlName(curr), getUrlName(heaviestUrl)) < 0) {
                    heaviestUrl = curr;
                }
            }
        }
        // Writes to file in format: Urlname, numOutlinks, pagerank
        fprintf(file, "%s, %d, %.7lf\n", getUrlName(heaviestUrl), numOutlinks(g, getUrlIndex(heaviestUrl)), pageRanks[getUrlIndex(heaviestUrl)]);
        visited[getUrlIndex(heaviestUrl)] = 1;
        ListItFree(it);
    }
    free(visited);
    fclose(file);
}

// Calculates pageranks for each url and puts that in pageRanks array
void pageRank(Graph g, double d, double diffPr, int maxIterations, double *pageRanks) {
    double nV = getNumVertices(g);
    double *prevPageRanks = malloc(sizeof(double) * nV);

    // Initializes pageRanks as all 1/N where N is number of urls
    for (int i = 0; i < nV; i++) {
        pageRanks[i] = 1/nV;
    }
    
    double diff = diffPr;
    int iteration = 0;
    while (iteration < maxIterations && diff >= diffPr) {
        // Copies pageRanks to prevPageRanks
        setPrevPageRanks(prevPageRanks, pageRanks, nV);

        // Calulates pagerank for each url in graph
        for (int i = 0; i < nV; i++) {
            double sum = 0;
            // Adds to sum for all links pointing to url excepting self loops
            for (int j = 0; j < nV; j++) {
                if (j != i && adjacent(g, j, i)) {
                    sum += prevPageRanks[j] * wIn(g, j, i) * wOut(g, j, i);
                }
            }
            // Pagerank formula
            pageRanks[i] = (1 - d)/nV + d * sum;
        }
        for (int i = 0; i < nV; i++) {
            printf("%f ", pageRanks[i]);
        }
        printf("\n");
        diff = calculateDiff(prevPageRanks, pageRanks, nV);
        iteration++;
    }

    free(prevPageRanks);
}

// Copies pageRanks to prevPageRanks
void setPrevPageRanks(double prevPageRanks[], double pageRanks[], int nV) {
    for (int i = 0; i < nV; i++) {
        prevPageRanks[i] = pageRanks[i];
    }
}

// Calculates the difference between prevPageRanks and PageRanks
double calculateDiff(double prevPageRanks[], double pageRanks[], int nV) {
    double diff = 0;
    for (int i = 0; i < nV; i++) {
        diff += fabs(pageRanks[i] - prevPageRanks[i]);
    }
    return diff;
}

// Calculates the wOut of a link
double wOut(Graph g, int index1, int index2) {
    double O_u = numOutlinks(g, index2);
    // Ensures that O_u is not zero
    O_u = O_u == 0 ? 0.5 : O_u;

    // Gets sum of the out degree of all outlinks of url at index1
    double sumO_p = 0;
    for (int i = 0; i < getNumVertices(g); i++) {
        if (i != index1 && adjacent(g, index1, i)) {
            int outlinks = numOutlinks(g, i);
            if (outlinks == 0) {
                sumO_p += 0.5;
            } else {
                sumO_p += outlinks;
            }
        }
    }
    return O_u/sumO_p;
}

// Counts all outlinks from url at given index
int numOutlinks(Graph g, int index) {
    int numV = getNumVertices(g);
    int count = 0;
    for (int i = 0; i < numV; i++) {
        if (i != index) {
            if (adjacent(g, index, i)) {
                count++;
            }
        }
    }
    return count;
}

// Calculates the wIn of a link
double wIn(Graph g, int index1, int index2) {
    double O_u = numInlinks(g, index2);
    // Ensures that O_u is not zero
    O_u = O_u == 0 ? 0.5 : O_u;

    // Gets sum of the out degree of all inLinks of url at index1
    double sumO_p = 0;
    for (int i = 0; i < getNumVertices(g); i++) {
        if (i != index1 && adjacent(g, index1, i)) {
            int outlinks = numInlinks(g, i);
            if (outlinks == 0) {
                sumO_p += 0.5;
            } else {
                sumO_p += outlinks;
            }
        }
    }
    return O_u/sumO_p;
}

// Counts all inLinks from url at given index
int numInlinks(Graph g, int index) {
    int numV = getNumVertices(g);
    int count = 0;
    for (int i = 0; i < numV; i++) {
        if (i != index) {
            if (adjacent(g, i, index)) {
                count++;
            }
        }
    }
    return count;
}
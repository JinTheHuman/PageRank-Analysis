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

#include "List.h"

#define MAX_URL_LENGTH 100
#define MAX_LINE_LENGTH 1000

void checkInverted(List Urls, char searchTerm[]);
void incrementUrl(List Urls, char urlName[]);
void returnSearchResultsOrdered(List Urls, int numUrls);
double getPageRank(Node url);
bool comesFirst(Node url1, Node url2);
int readInUrls(List Urls, char filename[]);

int main(int argc, char *argv[]) {
    List Urls = ListNew();
    // Reads in Urls from collection and adds to Urls
    int numUrls = readInUrls(Urls, "pagerankList.txt");

    // Check each search term against InvertedIndex and increment urls accordingly
    for (int i = 1; i < argc; i++) {
        checkInverted(Urls, argv[i]);
    }

    // Print Search results in order
    returnSearchResultsOrdered(Urls, numUrls);

    ListFree(Urls);
}

// Reads in urls from given filename and appends them to given list
int readInUrls(List Urls, char filename[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(1);
    }
    int numUrls = 0;
    char line[MAX_LINE_LENGTH];
    char *token;
    // Reads pagerankList.txt line by line
    while (fgets(line, sizeof(line), file)) {
        // Reads in the urlName from the line
        token = strtok(line, ", ");
        ListAppend(Urls, token, 0);
        numUrls++;
    }
    fclose(file);
    return numUrls++;
}

// Checks given search term against InvertedIndex and increments urls that include the term
void checkInverted(List Urls, char searchTerm[]) {
    FILE *file = fopen("invertedIndex.txt", "r");
    char line[MAX_LINE_LENGTH];
    char *token;
    // Iterates through lines of invertedIndex.txt
    while (fgets(line, sizeof(line), file)) {
        token = strtok(line, " ");
        // Compares search term against word index
        if (strcmp(token, searchTerm) == 0) {
            token = strtok(NULL, " ");
            // Reads all the url names next to the word and increments the urls
            while (token != NULL) {
                if (strcmp(token, "\n") != 0) {
                    incrementUrl(Urls, token);
                }
                token = strtok(NULL, " ");
            }
        }
    }
    fclose(file);
}

// Given the url name, iterates through the list of Urls and increments that url's index
void incrementUrl(List Urls, char urlName[]) {
    ListIterator it = ListItNew(Urls);
    Node curr;
    while (ListItHasNext(it)) {
        curr = ListItNext(it);
        if (strcmp(getUrlName(curr), urlName) == 0) {
            doIncrementUrl(curr);
        }
    }
    ListItFree(it);
}

// Prints search results in order of most relevant to least
void returnSearchResultsOrdered(List Urls, int numUrls) {
    // Does selection sort, getting the next most relevant result and printing it
    for (int i = 0; i < 30 && i < numUrls; i++) {
        ListIterator it = ListItNew(Urls);

        Node firstUrl = ListItNext(it);
        // Assures that initial firstUrl is not already printed
        while (isPrinted(firstUrl)) {
            firstUrl = ListItNext(it);
        }
        Node curr;
        // Searches for next most relevant search result
        while (ListItHasNext(it)) {
            curr = ListItNext(it);
            if (comesFirst(curr, firstUrl)) {
                firstUrl = curr;
            }
        }
        ListItFree(it);
        // Prints result given that the url's index is not 0
        if (getUrlIndex(firstUrl) != 0) {
            printName(firstUrl);
        } else {
            break;
        }
    }
}

// Gets page rank of given url
double getPageRank(Node url) {
    char name[100];
    strcpy(name, getUrlName(url));
    FILE *file = fopen("pagerankList.txt", "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(1);
    }
    
    char line[MAX_LINE_LENGTH];
    char *token;
    // Reads pagerankList.txt line by line
    while (fgets(line, sizeof(line), file)) {
        // Reads in the urlName from the line
        token = strtok(line, ", ");
        if (strcmp(token, name) == 0) {
            // Reads in next word in line which is the number of inLinks
            token = strtok(NULL, ", ");
            // Reads in next word in line which is the PageRank
            token = strtok(NULL, ", ");
            fclose(file);
            return atof(token);
        }
    }
    fclose(file);
    return -1;
}


// Compares two nodes and returns whether url1 is more relevant than url2
bool comesFirst(Node url1, Node url2) {
    // Compares Index
    if (getUrlIndex(url1) > getUrlIndex(url2) && !isPrinted(url1)) {
        return true;
    } // If indexes are equal compares Pageranks
    else if (getUrlIndex(url1) == getUrlIndex(url2) && !isPrinted(url1)) {
        if (getPageRank(url1) > getPageRank(url2)) {
            return true;
        } // If Page ranks are equal compares ascii order 
        else if (getPageRank(url1) == getPageRank(url2)) {
            if (strcmp(getUrlName(url1), getUrlName(url2)) < 0) {
                return true;
            }
        }
    }
    return false;
}
// Graph ADT
// Adjacency Matrix Representation ... COMP2521
// This is taken from Lecture code and has been modified to suit my project
#include "Graph.h"
#include "List.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct GraphRep {
   int  **edges;   // adjacency matrix
   int    nV;      // #vertices
   int    nE;      // #edges
   List Urls;
} GraphRep;

Graph newGraph(int V) {
   assert(V >= 0);
   int i;

   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = V;
   g->nE = 0;

   // allocate memory for each row
   g->edges = malloc(V * sizeof(int *));
   assert(g->edges != NULL);
   // allocate memory for each column and initialise with 0
   for (i = 0; i < V; i++) {
      g->edges[i] = calloc(V, sizeof(int));
      assert(g->edges[i] != NULL);
   }

   return g;
}

// check if vertex is valid in a graph
bool validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}

void insertEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (!g->edges[e.v][e.w]) {  // edge e not in graph
      g->edges[e.v][e.w] = 1;
      g->nE++;
   }
}

bool adjacent(Graph g, Vertex v, Vertex w) {
   assert(g != NULL && validV(g,v) && validV(g,w));

   return (g->edges[v][w] != 0);
}

void freeGraph(Graph g) {
   assert(g != NULL);
   ListFree(g->Urls);
   int i;
   for (i = 0; i < g->nV; i++)
      free(g->edges[i]);
   free(g->edges);
   free(g);
}

void assignList(Graph g, List l) {
    g->Urls = l;
}

List getGraphUrls(Graph g) {
    return g->Urls;
}

int findUrlIndex(Graph g, char theUrlName[]) {
    ListIterator it = ListItNew(getGraphUrls(g));
    Node curr;
    while (ListItHasNext(it)) {
        curr = ListItNext(it);
        if (strcmp(getUrlName(curr), theUrlName) == 0) {
            ListItFree(it);
            return getUrlIndex(curr);
        }
    }
    return -1;
}

int getNumVertices(Graph g) {
    return g->nV;
}
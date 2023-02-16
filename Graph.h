// Graph ADT interface ... COMP2521
// This is taken from Lecture code and has been modified to suit my project
#include <stdbool.h>
#include "List.h"

typedef struct GraphRep *Graph;

// vertices are ints
typedef struct UrlRep {
    char name[100];
    int index;
} UrlRep;

typedef struct UrlRep *Url;

typedef int Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   Vertex v;
   Vertex w;
} Edge;

Graph newGraph(int);
void  insertEdge(Graph, Edge);
bool  adjacent(Graph, Vertex, Vertex);
void  freeGraph(Graph);
void assignList(Graph g, List l);
List getGraphUrls(Graph g);
int findUrlIndex(Graph g, char theUrlName[]);
int getNumVertices(Graph g);
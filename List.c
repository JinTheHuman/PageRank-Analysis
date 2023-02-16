// This is the implemenatat of a Url List ADT,
// where the nodes hold the url's name, index, printed and next
// This is taken from Lecture code and has been modified to suit my project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"
#include "Graph.h"

typedef struct node *Node;
struct node {
    char  *urlName;
    int index;
    bool printed;
    Node   next;
};

struct list {
    Node   head;
    Node   tail;
    int    size;
};

static Node newNode(char *urlName, int theIndex);
static char *myStrdup(char *urlName);

////////////////////////////////////////////////////////////////////////

// Creates a new empty list
List ListNew(void) {
    List l = malloc(sizeof(*l));
    if (l == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
}

// Frees all memory allocated for the given list
void ListFree(List l) {
    Node curr = l->head;
    while (curr != NULL) {
        Node temp = curr;
        curr = curr->next;
        free(temp->urlName);
        free(temp);
    }
    free(l);
}

// Adds a string to the end of the list
void ListAppend(List l, char *urlName, int theIndex) {
    Node n = newNode(urlName, theIndex);
    if (l->head == NULL) {
        l->head = n;
    } else {
        l->tail->next = n;
    }
    l->tail = n;
    l->size++;
}

static Node newNode(char *urlName, int theIndex) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    n->urlName = myStrdup(urlName);
    n->index = theIndex;
    n->printed = false;
    n->next = NULL;
    return n;
}

static char *myStrdup(char *urlName) {
    char *copy = malloc((strlen(urlName) + 1) * sizeof(char));
    if (copy == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return strcpy(copy, urlName);
}

////////////////////////////////////////////////////////////////////////

struct listIterator {
    Node curr;
    List list;
};

// Creates an iterator for the given list
ListIterator ListItNew(List l) {
    ListIterator it = malloc(sizeof(*it));
    if (it == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    it->curr = l->head;
    it->list = l;
    return it;
}

// Gets the next item in the list. The item should not be modified.
Node ListItNext(ListIterator it) {
    if (it->curr == NULL) {
        fprintf(stderr, "error: no more items in iterator!\n");
        exit(EXIT_FAILURE);
    }

    //char *item = it->curr->urlName;
    Node returnCurr = it->curr;
    it->curr = it->curr->next;
    return returnCurr;
}

// Checks if the list has a next item
bool ListItHasNext(ListIterator it) {
    return it->curr != NULL;
}

// Frees the given iterator
void ListItFree(ListIterator it) {
    free(it);
}

int getUrlIndex(Node url) {
    return url->index;
}

char * getUrlName(Node url) {
    return url->urlName;
}

void doIncrementUrl(Node url) {
    url->index++;
}

bool isPrinted(Node url) {
    return url->printed;
}

void printName(Node url) {
    printf("%s\n", url->urlName);
    url->printed = true;
}
// CHANGE THIS BACK
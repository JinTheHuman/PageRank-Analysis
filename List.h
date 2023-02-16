// This is the Interface of a Url List ADT,
// where the nodes hold the url's name, index, printed and next
// This is taken from Lecture code and has been modified to suit my project

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct list *List;
typedef struct node *Node;

// Creates a new empty list
// Complexity: O(1)
List ListNew(void);

// Frees all memory allocated for the given list
// Complexity: O(n)
void ListFree(List l);

// Adds a string to the end of the list. Makes a copy of the string
// before adding it to the list.
// Complexity: O(1)
void ListAppend(List l, char *s, int theIndex);

////////////////////////////////////////////////////////////////////////
// Do NOT use these functions

typedef struct listIterator *ListIterator;

// Creates an iterator for the given list
// Complexity: O(1)
ListIterator ListItNew(List l);

// Gets the next item in the list. The item should not be modified.
// Complexity: O(1)
Node ListItNext(ListIterator it);

// Checks if the list has a next item
// Complexity: O(1)
bool ListItHasNext(ListIterator it);

// Frees the given iterator
// Complexity: O(1)
void ListItFree(ListIterator it);

int getUrlIndex(Node url);
char * getUrlName(Node url);
void doIncrementUrl(Node url);
bool isPrinted(Node url);
void printName(Node url);

#endif


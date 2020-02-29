#include"bubblesort.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

// Initilization of strictures
struct node {item x; struct node *next; struct node *back; };
typedef struct node node;

struct list {node *prenode; node *current; node *end; item def;};
typedef struct list list;

// Create a new empty list, with a default item to return in case of error.
list *newList(item d){
    list *l = malloc(sizeof(list));
    node *newNodeEnd=malloc(sizeof(node));
    node *newPreNode=malloc(sizeof(node));

    l->prenode=newPreNode;
    l->prenode->next=newNodeEnd;
    l->end=newNodeEnd;
    newNodeEnd->next=NULL;
    newNodeEnd->back=newPreNode;
    l->current=l->end;
    l->def=d;

    return l;
}

// Free up the list and all the data in it.
void freeList(list *l){
    node *n=l->prenode;
    while (n!=NULL){
        node *next= n->next;
        free(n);
        n=next;
    }
    free(l);
}


// Set the current position before the first item or after the last item,
// to begin a forward or backward traversal.
void startF(list *l){
    l->current=l->prenode->next;
}
void startB(list *l){
    l->current=l->end;
}


// Check whether the current position is at the end or start, to test
// whether a traversal has finished.
bool endF(list *l){
    if (l->current==l->end) return true;
    return false;
}
bool endB(list *l){
    if (l->current==l->prenode->next) return true;
    return false;
}


// Move the current position one place forwards or backwards, and return true.
// If nextF is called when at the end of the list, or nextB when at the start,
// the functions do nothing and return false.
bool nextF(list *l){
    if (endF(l)==true)  return false;

    l->current=l->current->next;
    return true;
}
bool nextB(list *l){
    if (endB(l)==true)  return false;

    l->current=l->current->back;
    return true;
}


// Insert an item before the current position during a traversal.  The traversal
// of the remaining items is not affected.
void insertF(list *l, item x){
    node *new=malloc(sizeof(node));

    new->x=x;
    l->current->back->next=new;
    new->back=l->current->back;
    new->next=l->current;
    l->current->back=new;
}
void insertB(list *l, item x){
    insertF(l,x);
    l->current=l->current->back;
}


// Get the current item. If getF is called when at the end the default item is returned.
item getF(list *l){
    if (endF(l)==true)  return l->def;
    return l->current->x;
}


//----------------------------BUBBLE SORT FUNCTIONS-----=-----------------------

//Returns the number of nodes in a list (without the pre&end node)
item ListLength(list *l){
    node *cur=l->current;
    l->current=l->prenode->next;
    int i=0;
    while (!endF(l)){
        i+=1;
        nextF(l);
    }
    l->current=cur;
    return i;
}

//Swaps two completely two nodes of a list, specifically it swaps the current position node with the next
//In the case the list is not big enough returns false
bool Swap(list *l){
    if (ListLength(l)<2) return false;

    node *smaller=l->current->next;
    node *bigger=l->current;
    if (bigger->back==l->prenode)  l->prenode->next=smaller; //
    node *x=smaller->next;
    node *y=bigger->next;
    node *w=bigger->back;
    smaller->next=smaller->back;
    bigger->next=x;
    smaller->back=w;
    bigger->back=y;
    x->back=smaller->next;
    smaller->back->next=smaller;

    return true;
}

//Sorts a doubly linked list with bubblesort implementation (ascending order)
//In the case the list is not big enough returns false
bool Bubblesort(list *l){
    node *cur=l->current;
    int length= ListLength(l);
    if (length<2)  return false;

    for (int i=0; i<length-1; i++){
        for (int j=0; j< length-i-1; j++){
            l->current=l->prenode->next;
            for (int k=0; k<j; k++)  nextF(l);
            if ((l->current->x)>(l->current->next->x))  Swap(l);
        }
    }
    l->current=cur;
    return true;
}
//---------------------------------------------------------------------------




//=============================================================================
// Testing for the lists module. Strings are used to describe lists. The strings
// "|37", "3|7", "37|" represent a list of two items, with the current position
// at the start, middle or end.
#ifdef test_lists

// Convert a string description to a list.
static list *toList(char *pic) {
    int pos = strchr(pic, '|') - pic;
    list *l = newList(-1);
    for (int i = 0; i < strlen(pic); i++) {
        if (pic[i] == '|') continue;
        int n = pic[i] - '0';
        insertF(l, n);
    }
    startF(l);
    for (int i = 0; i < pos; i++){
        nextF(l);
    }
    return l;
}

// Convert a list to a string description.
static void toString(list *l, char s[]) {
    int pos = 0;
    while (! endB(l)) { pos++; nextB(l); }
    int i = 0;
    while (! endF(l)) {
        if (i == pos) s[i++] = '|';
        s[i++] = '0' + getF(l);
        nextF(l);
    }
    if (i == pos) s[i++] = '|';
    s[i++] = '\0';
}

// Check a particular function. The first argument is the name of the function.
// The second and third arguments describes the list before and after the call.
// The fourth, fifth and sixth arguments are an item to pass as an argument, an
// item to expect as a result, and a boolean to expect as a result, if any.
static bool check(char *op, char *lb, char *la, item x, item y, bool b) {
    bool r = true;
    item z = 0;
    list *l = toList(lb);
    if (strcmp(op, "ListLength")==0) z=ListLength(l);
    else if (strcmp(op, "Swap")==0) r=Swap(l);
    else if (strcmp(op, "Bubblesort")==0) r=Bubblesort(l);
    else return false;
    if (r != b || z != y) return false;
    char s[100];
    toString(l, s);
    freeList(l);
    return strcmp(s, la) == 0;
}


//--------------------------BUBLE SORT FUNCTIONS TESTING------------------------
static void testLength() {
    assert(check("ListLength", "|", "|", 0, 0, true));
    assert(check("ListLength", "|7", "|7", 0, 1, true));
    assert(check("ListLength", "|37", "|37", 0, 2, true));
    assert(check("ListLength", "37|", "37|", 0, 2, true));
    assert(check("ListLength", "37|542", "37|542", 0, 5, true));
}

static void testSwap() {
    assert(check("Swap", "|", "|", 0, 0, false));
    assert(check("Swap", "|9", "|9", 0, 0, false));
    assert(check("Swap", "|97", "7|9", 0, 0, true));
    assert(check("Swap", "3|78", "38|7", 0, 0, true));
    assert(check("Swap", "123|70", "1230|7", 0, 0, true));
}

static void testBubblesort(){
    assert(check("Bubblesort", "|", "|", 0, 0, false));
    assert(check("Bubblesort", "|1", "|1", 0, 0, false));
    assert(check("Bubblesort", "|41", "1|4", 0, 0, true));
    assert(check("Bubblesort", "|523", "23|5", 0, 0, true));
    assert(check("Bubblesort", "|888", "|888", 0, 0, true));
    assert(check("Bubblesort", "089319|", "013899|", 0, 0, true));
}
//------------------------------------------------------------------------------

int main() {
    testLength();
    testSwap();
    testBubblesort();
    printf("Bubblesort of doubly linked lists module OK\n");
    return 0;
}

#endif

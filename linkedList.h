#ifndef XYZ_LINKEDLIST_H
#define XYZ_LINKEDLIST_H

typedef struct tnode {
    int c;
    int freq;
    struct tnode* right;
    struct tnode* left;
    struct tnode* parent;
} tnode;

typedef struct node{
    tnode* value;
    struct node* next;
}PriorityLinkedList;

PriorityLinkedList* llCreate();
void llDisplay(PriorityLinkedList*);
void llAdd(PriorityLinkedList**, tnode*);
int llSize(PriorityLinkedList*);
void llFree(PriorityLinkedList*);
void llRemove_At_Index(PriorityLinkedList**, int n);
void llAdd_in_order(PriorityLinkedList**, tnode*);
#endif

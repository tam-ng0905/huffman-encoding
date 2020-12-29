#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedList.h"

//Creates a LinkedList
PriorityLinkedList* llCreate(){
    return NULL;
}
//Displays the c values of every node in LinkedList
void llDisplay(PriorityLinkedList* ll){
    PriorityLinkedList* p  = ll;

    printf("[");
    while (p != NULL) {
        printf(" %d ", p->value->c);
        p = p->next;
    }
    printf("]\n");
}
//Adds a link containing a tree node to LinkedList
void llAdd (PriorityLinkedList** ll, tnode* str) {

    //allocate memory for the link
    PriorityLinkedList* newNode = (PriorityLinkedList*)malloc(sizeof(PriorityLinkedList));
    newNode->value = str;
    newNode->next = NULL;

    //Find the end of the list
    PriorityLinkedList* p = *ll;

    if(p == NULL) {
        *ll = newNode;
    } else{
        while(p->next != NULL){
            p = p->next;
        }
        p->next = newNode;
    }
}
//Returns the size of LinkedList
int llSize(PriorityLinkedList *ll){
    PriorityLinkedList* p = ll;
    int size = 0;

    if(p == NULL){
        return size;
    }

    while(p != NULL){
        p = p->next;
        size++;
    }
    return size;
}

void llRemove_At_Index(PriorityLinkedList** ll, int index){
    int size = llSize(*ll);
    if(*ll != NULL && index < size){
        PriorityLinkedList* p = *ll;
        if(index == 0){
            *ll = p->next;
            free(p);
        }else{
            int i = 0;
            while(p != NULL && i <  index - 1){
                p = p->next;
                i++;
            }

            PriorityLinkedList* c = p->next;
            PriorityLinkedList* a = c->next;
            p->next = a;
            free(c);
        }
    }
}

void llFree(PriorityLinkedList* ll) {
    PriorityLinkedList* p = ll;
    while (p != NULL) {
        PriorityLinkedList* oldP = p;
        p = p->next;
        free(oldP);
    }

}

void llAdd_in_order(PriorityLinkedList** pll, tnode* newValue){

    PriorityLinkedList* newNode = (PriorityLinkedList*)malloc(1 * sizeof(PriorityLinkedList));
    newNode->value = newValue;
    newNode->next = NULL;
    if (*pll == NULL || (*pll)->value->freq >= newNode->value->freq)
    {
        newNode->next = *pll;
        *pll = newNode;
        return;
    }
    PriorityLinkedList* current = *pll;
    while (current->next != NULL && current->next->value->freq < newNode->value->freq)
        current = current->next;

    newNode->next = current->next;
    current->next = newNode;
}


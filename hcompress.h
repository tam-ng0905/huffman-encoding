#ifndef XYZ_HCOMPRESS_H
#define XYZ_HCOMPRESS_H
#include "linkedList.h"
PriorityLinkedList* createFreqTable(char*);
struct tnode* createHuffmanTree(PriorityLinkedList*);
void encodeFile(char*, PriorityLinkedList*);
void decodeFile(char*, struct tnode*);
char* decToBin(int);
void clearLinkedList(PriorityLinkedList*);
void recursAdding(tnode*);
#endif

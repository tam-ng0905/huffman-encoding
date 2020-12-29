#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hcompress.h"
#include "linkedList.h"

int main(int argc, char *argv[]) {

    // Check the make sure the input parameters are correct

    if (argc != 3) {
        printf("Error: The correct format is \"hcompress -e filename\" or \"hcompress -d filename.huf\"\n"); fflush(stdout);

        exit(1);

    }

    // Create the frequency table by reading the generic file

    PriorityLinkedList* leafNodes = createFreqTable("decind.txt");

    // Create the huffman tree from the frequency table

    struct tnode* treeRoot = createHuffmanTree(leafNodes);

    // encode

    if (strcmp(argv[1], "-e") == 0) {

        // Pass the leafNodes since it will process bottom up

        encodeFile(argv[2], leafNodes);

    } else { // decode

        // Pass the tree root since it will process top down

        decodeFile(argv[2], treeRoot);

    }

    free(treeRoot);
    llFree(leafNodes);

    return 0;

}

PriorityLinkedList* createFreqTable(char* file){

    //Get the file opened
    FILE* f;
    f = fopen(file, "r");

    //declaring some necessary variables for the functions
    char c;
    int size = 129;
    int charString[size];

    PriorityLinkedList* leafNodes = (PriorityLinkedList*) malloc(127 * sizeof(PriorityLinkedList));
    leafNodes = llCreate();


    //Set all the fre = 0
    for(int i = 0; i < size; i++) {
        charString[i] = 0;
    }

    //read in the content of the file
    while((c = fgetc(f)) != EOF) {
        charString[c]++;
    }

    charString[128]++;

    for(int i = 0; i < size; i++){
        tnode* newValue = (tnode*)malloc(sizeof(tnode));
        newValue->c = i;
        newValue->freq = charString[i];
        llAdd_in_order(&leafNodes, newValue);
    }

    fclose(f);

    return leafNodes;

}

tnode* createHuffmanTree(PriorityLinkedList* leafNodes){

    //declaring some necessary variables for the functions
    tnode* temp;
    PriorityLinkedList* a = leafNodes;
    while (leafNodes->next != NULL) {

        //allocate the root
        temp = (tnode*)malloc(sizeof(tnode));
        temp->c = -1;


        //connect the node to the root
        leafNodes->value->parent = temp;
        temp->right = leafNodes->value;
        leafNodes = leafNodes->next;
        leafNodes->value->parent = temp;
        temp->left = leafNodes->value;
        leafNodes = leafNodes->next;
        temp->freq = (temp->left)->freq + (temp->right)->freq;
        llAdd_in_order(&leafNodes, temp);

    }
    //assign the 1s and 0s to the tree
    recursAdding(temp);

    leafNodes = a;
    int i = 0;


    PriorityLinkedList* p = leafNodes;
    while(p != NULL) {
        if (p->value->c == -1) {
            llRemove_At_Index(&leafNodes, i);
            i--;
        }
        i++;
        p = p->next;
    }
    return temp;
}
void encodeFile(char* file, PriorityLinkedList* leafNodes){
    //declaring some necessary variables for the functions
    FILE* f;
    FILE* fw;
    tnode* current;
    PriorityLinkedList* p;
    int charCode[100];
    int stringBit[8];
    unsigned char buffer = 0;
    int bCount = 0;
    int strSize;
    int index;
    char c;
    int writes = 0;


    //read in the files if the file exists
    if ((f = fopen(file, "r")) != NULL) {
        fw = fopen(strcat(file, ".huf"), "w+");

        //get the content from the file
        while((c = fgetc(f)) != EOF) {
            strSize = 0;
            p = leafNodes;
            while (p->value->c != c) {
                p = p->next;
            }
            current = p->value->parent;

            if ((current->right->c) == (p->value->c)) {
                charCode[strSize] = 1;
            } else {
                charCode[strSize] = 0;
            }
            strSize++;

            //append the 1 and 0
            while (current->parent != NULL) {
                charCode[strSize] = current->freq;
                current = current->parent;
                strSize++;

            }

            int changeString[strSize];
            for (int i = 0; i < strSize; i++) {
                changeString[i] = charCode[strSize - i - 1];
            }

            for(int j = 0; j < strSize; j++) {

                stringBit[bCount] = changeString[j];

                bCount++;
                if (bCount == 8) {
                    index = 0;
                    while(index < 8) {
                        buffer = buffer << 1;
                        if (stringBit[index] == 1) {
                            buffer = buffer | 1;
                        }
                        index++;
                    }
                    putc(buffer, fw);
                    writes++;
                    bCount = 0;
                    buffer = 0;
                }

            }


        }
        int bits = 0;
        strSize = 0;
        p = leafNodes;

        while (p->value->c != 128) {
            p = p->next;
        }
        current = p->value->parent;
        if ((current->right->c) == (p->value->c)) {
            charCode[strSize] = 1;
        } else {
            charCode[strSize] = 0;
        }

        strSize++;
        while (current->parent != NULL) {

            charCode[strSize] = current->freq;
            current = current->parent;
            strSize++;

        }
        int changeString[strSize];
        for (int i = 0; i < strSize; i++) {
            changeString[i] = charCode[strSize - i - 1];
        }


        for(int j = 0; j < strSize; j++) {
            stringBit[bCount] = changeString[j];
            bits++;

            bCount++;
            //Write the file if it's full
            if (bCount == 8) {
                bits = 0;
                index = 0;
                while(index < 8) {
                    buffer = buffer << 1;
                    if (stringBit[index] == 1) {
                        buffer = buffer | 1;
                    }
                    index++;

                }
                putc(buffer, fw);
                writes++;
                bCount = 0;
                buffer = 0;
            }
        }
        if (bits > 0) {
            for (int j = 0; j < bits; j++) {
                buffer = buffer << 1;
                if (stringBit[j] == 1) {
                    buffer = buffer | 1;
                }
            }
            buffer = buffer << (8 - bits);
            putc(buffer, fw);
            fclose(fw);
        }
        fclose(f);
    } else {
        printf("Please enter another file");
    }
}


void decodeFile(char* file, tnode* temp){

    FILE* f;
    int isFinished = 1;
    char c;
    char* bi;
    int count = 0;
    int index = 0;

    if ((f = fopen(file, "rb")) == NULL) {
        printf("Please enter another file\n");
    } else {

        //calculate the size of the file
        FILE* fw;
        fw = fopen(strcat(file, ".dec"), "w+");
        fseek(f, 0, SEEK_END);
        long charIndex = ftell(f);
        unsigned char characters[charIndex];


        tnode* value = temp;
        rewind(f);
        fread(characters, sizeof(char), charIndex, f);
        while(index < charIndex && isFinished == 1) {
            c = characters[index];
            count = 0;
            bi = decToBin(c);
            while(count <= 7 && isFinished == 1){
                if(bi[count] == '1'){
                    value = value->right;
                }else if (bi[count] == '0'){
                    value = value->left;
                }
                if(value->c == 128){
                    isFinished = 0;
                }
                if(isFinished == 1 && value->right == NULL && value->left == NULL){
                    putc(value->c, fw);
                    value = temp;
                }
                count++;
            }
            index++;
            free(bi);
        }
        fclose(fw);
    }
    fclose(f);
}

char* decToBin(int dec){
    if(dec < 0)
        dec += 256;

    char* bin = (char*)malloc(8*sizeof(char));

    for(int i = 0; i < 8; i++){
        if(dec - (pow(2, 8 - 1 - i)) >= 0){
            *(bin + i) = '1';
            dec = dec - (pow(2, 8 - 1 - i));
        }else
            *(bin+i) = '0';
    }
    return bin;
}
void recursAdding(tnode* value) {

    if (value->right->c == -1) {
        value->right->freq = 1;
        recursAdding(value->right);
    }
    if (value->left->c == -1) {
        value->left->freq = 0;
        recursAdding(value->left);
    }
}

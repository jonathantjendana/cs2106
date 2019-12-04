#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2 // New instruction compared to ex1.

// Node and SubNode declarations ( Do not Edit )
typedef struct SUBNODE {
    int data;
    struct SUBNODE *nextSubNode;
} subNode;

typedef struct NODE {
    int data;
    struct NODE *previousNode;
    struct NODE *nextNode;
    subNode *subNodeHead;
} node;

// Function prototypes
void insertNodeNext(int, int, node *);     // TODO
void insertNodePrevious(int, int, node *); // TODO
void deleteNode(int, node *);              // TODO
void deleteList(node *);                   // TODO
void insertSubNode(int, int, int, node *); // TODO
void deleteAllSubNodes(node *); // Optional TODO: will be useful in ex3.

void printList(node *);
void printSubNodes(node *);

// Start of main
int main() {
    int position;
    int instruction;
    int subPosition;
    int value;

    // Declaration of the origin Node ( do not edit )
    node *originNode = (node *)malloc(sizeof(node));
    originNode->previousNode = originNode;
    originNode->nextNode = originNode;
    originNode->data = 0;
    originNode->subNodeHead = NULL;

    // input and processing has been done for you
    while (scanf("%i%i", &instruction, &position) == 2)
    // scanf() returns an int that corresponds to the number of values scanned.
    {
        if (instruction == DELETE) {
            deleteNode(position, originNode);
        } else if (instruction == INSERTSUBNODE) {
            scanf("%i%i", &subPosition, &value);
            insertSubNode(position, subPosition, value, originNode);
        } else if (instruction == NEXT) {
            scanf("%i", &value);
            insertNodeNext(position, value, originNode);
        } else if (instruction == PREVIOUS) {
            scanf("%i", &value);
            insertNodePrevious(position, value, originNode);
        }
    }

    printList(originNode);
    deleteList(originNode);

    printf("Circular List after delete\n");
    printList(originNode);

    free(originNode);
    return 0;
}

// Function Implementations
void insertSubNode(int position, int subPosition, int value, node *originNode) {
    while (position--) {
        originNode = originNode->nextNode;
    }
    subNode *curr = (subNode *)malloc(sizeof(subNode));
    curr->data = value;
    curr->nextSubNode = NULL;
    if (!subPosition) {
        // subNodeHead
        subNode *subref = originNode->subNodeHead;
        originNode->subNodeHead = curr;
        curr->nextSubNode = subref;
    } else {
        subNode *befsubref = originNode->subNodeHead;
        while (--subPosition) {
            befsubref = befsubref->nextSubNode;
        }
        curr->nextSubNode = befsubref->nextSubNode;
        befsubref->nextSubNode = curr;
    }
}

void insertNodePrevious(int position, int value, node *originNode) {
    while (position--) {
        originNode = originNode->nextNode;
    }
    node *curr = (node *)malloc(sizeof(node));
    curr->data = value;
    curr->nextNode = originNode;
    curr->previousNode = originNode->previousNode;
    curr->subNodeHead = NULL;
    curr->nextNode->previousNode = curr;
    curr->previousNode->nextNode = curr;
}

void insertNodeNext(int position, int value, node *originNode) {
    while (position--) {
        originNode = originNode->nextNode;
    }
    node *curr = (node *)malloc(sizeof(node));
    curr->data = value;
    curr->nextNode = originNode->nextNode;
    curr->previousNode = originNode;
    curr->subNodeHead = NULL;
    curr->nextNode->previousNode = curr;
    curr->previousNode->nextNode = curr;
}

void deleteNode(int position, node *originNode) {
    while (position--) {
        originNode = originNode->nextNode;
    }
    originNode->nextNode->previousNode = originNode->previousNode;
    originNode->previousNode->nextNode = originNode->nextNode;
    if (originNode->subNodeHead) {
        deleteAllSubNodes(originNode);
    }
    free(originNode);
}

void deleteAllSubNodes(node *targetNode) {
    subNode *sub = targetNode->subNodeHead;
    while (sub) {
        subNode *ref = sub->nextSubNode;
        free(sub);
        sub = ref;
    }
    targetNode->subNodeHead = NULL;
}

void deleteList(node *originNode) {
    node *ref = originNode->nextNode;
    while (ref != originNode) {
        node *aft = ref->nextNode;
        deleteNode(1, originNode);
        ref = aft;
    }
    originNode->nextNode = originNode;
    originNode->previousNode = originNode;
    deleteAllSubNodes(originNode);
}

// Print list has been implemented for you.
// You may study the functions below to help you implement your own functions.
void printList(node *originNode) {
    int count = 0;
    printf("Printing clockwise:\n");
    node *iterator = originNode->nextNode;
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    printSubNodes(originNode);
    printf("\n   |\n   v\n");
    while (iterator != originNode) {
        count++;
        printf("[Pos %i:%i]", count, iterator->data);
        // printing subNodes
        printSubNodes(iterator);
        printf("\n   |\n   v\n");
        iterator = iterator->nextNode;
    }
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    printSubNodes(originNode);

    printf("\nPrinting counter-clockwise:\n");
    iterator = originNode->previousNode;
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    printSubNodes(originNode);
    printf("\n   |\n   v\n");
    while (iterator != originNode) {
        printf("[Pos %i:%i]", count, iterator->data);
        // printing subNodes
        printSubNodes(iterator);
        printf("\n   |\n   v\n");
        iterator = iterator->previousNode;
        count--;
    }
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    printSubNodes(originNode);
    printf("\n");

    return;
}

void printSubNodes(node *mainNode) {
    int count = 0;
    subNode *iterator = mainNode->subNodeHead;
    while (iterator != NULL) {
        printf("->[subNode pos %i:%i]", count, iterator->data);
        iterator = iterator->nextSubNode;
        count++;
    }
}

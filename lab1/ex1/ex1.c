#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0

// Node declaration ( Do not edit )
typedef struct NODE {
    int data;
    struct NODE *previousNode;
    struct NODE *nextNode;
} node;

// Function prototypes
void insertNodeNext(int, int, node *);     // TODO
void insertNodePrevious(int, int, node *); // TODO
void deleteNode(int, node *);              // TODO
void deleteList(node *);                   // TODO

void printList(node *); // Implemented for you

// Start of main
int main() {
    int position;
    int instruction;
    int value;

    // Declaration of the origin Node ( do not edit )
    node *originNode = (node *)malloc(sizeof(node));
    originNode->previousNode = originNode;
    originNode->nextNode = originNode;
    originNode->data = 0;

    // input and processing has been done for you.
    // scanf() returns an int that corresponds to the number of values scanned.
    while (scanf("%i%i", &instruction, &position) == 2) {
        if (instruction == DELETE) {
            deleteNode(position, originNode);
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
void insertNodePrevious(int position, int value, node *originNode) {
    node *ref = originNode;
    while (position--) {
        ref = ref->nextNode;
    }
    node *bef = ref->previousNode;
    node *curr = (node *)malloc(sizeof(node));
    curr->data = value;
    curr->nextNode = ref;
    curr->previousNode = bef;
    ref->previousNode = curr;
    bef->nextNode = curr;
}

void insertNodeNext(int position, int value, node *originNode) {
    node *ref = originNode;
    while (position--) {
        ref = ref->nextNode;
    }
    node *aft = ref->nextNode;
    node *curr = (node *)malloc(sizeof(node));
    curr->data = value;
    curr->nextNode = aft;
    curr->previousNode = ref;
    ref->nextNode = curr;
    aft->previousNode = curr;
}

void deleteNode(int position, node *originNode) {
    node *ref = originNode;
    while (position--) {
        ref = ref->nextNode;
    }
    node *aft = ref->nextNode;
    node *bef = ref->previousNode;
    aft->previousNode = bef;
    bef->nextNode = aft;
    free(ref);
}

void deleteList(node *originNode) {
    node *ref = originNode->nextNode;
    while (ref != originNode) {
        node *aft = ref->nextNode;
        free(ref);
        ref = aft;
    }
    originNode->nextNode = originNode;
    originNode->previousNode = originNode;
}

// printList has been implemented for you.
// You may study the functions below to help you implement your own functions.
void printList(node *originNode) {
    int count = 0;

    printf("Printing clockwise:\n");
    node *iterator = originNode->nextNode;
    printf("[Pos %i:%i]", 0, originNode->data);
    printf("\n   |\n   v\n");
    while (iterator != originNode) {
        count++;
        printf("[Pos %i:%i]", count, iterator->data);
        printf("\n   |\n   v\n");
        iterator = iterator->nextNode;
    }
    printf("[Pos %i:%i]", 0, originNode->data);

    printf("\nPrinting counter-clockwise:\n");
    iterator = originNode->previousNode;
    printf("[Pos %i:%i]", 0, originNode->data);
    printf("\n   |\n   v\n");
    while (iterator != originNode) {
        printf("[Pos %i:%i]", count, iterator->data);
        printf("\n   |\n   v\n");
        iterator = iterator->previousNode;
        count--;
    }
    printf("[Pos %i:%i]\n", 0, originNode->data);

    return;
}

#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2
#define COLLAPSE 3 // New instruction compared to ex2.

// Node declarations

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

void insertNodeNext(int, int, node *);
void insertNodePrevious(int, int, node *);
void deleteNode(int, node *);
void printList(node *);
void printSubNodes(node *);
void deleteList(node *);
void insertSubNode(int, int, int, node *);
void deleteAllSubNodes(node *);
void collapseSubNodes(int, node *); // TODO

void (*fptr[9])() = {insertNodeNext, insertNodePrevious, deleteNode,
                     printList,      printSubNodes,      deleteList,
                     insertSubNode,  deleteAllSubNodes,  collapseSubNodes};

// Start of main
int main() {
    int position;
    int instruction;
    int subPosition;
    int value;

    // Declaration of the origin Node
    node *originNode = (node *)malloc(sizeof(node));
    originNode->previousNode = originNode;
    originNode->nextNode = originNode;
    originNode->data = 0;
    originNode->subNodeHead = NULL;

    // scanf() returns an int that corresponds to the number of values scanned.
    while (scanf("%i%i", &instruction, &position) == 2) {
        if (instruction == DELETE) {
            (*fptr[2])(position, originNode);
        } else if (instruction == INSERTSUBNODE) {
            scanf("%i%i", &subPosition, &value);
            (*fptr[6])(position, subPosition, value, originNode);
        } else if (instruction == NEXT) {
            scanf("%i", &value);
            (*fptr[0])(position, value, originNode);
        } else if (instruction == PREVIOUS) {
            scanf("%i", &value);
            (*fptr[1])(position, value, originNode);
        } else if (instruction == COLLAPSE) {
            (*fptr[8])(position, originNode);
        }
    }
    (*fptr[3])(originNode);
    (*fptr[5])(originNode);

    printf("Circular List after delete\n");
    (*fptr[3])(originNode);

    free(originNode);

    return 0;
}

void collapseSubNodes(int position, node *originNode) {
    while (position--) {
        originNode = originNode->nextNode;
    }
    subNode *ref = originNode->subNodeHead;
    int sum = 0;
    while (ref) {
        sum += ref->data;
        subNode *tmp = ref->nextSubNode;
        free(ref);
        ref = tmp;
    }
    originNode->data += sum;
    originNode->subNodeHead = NULL;
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

void deleteAllSubNodes(node *targetNode) {
    subNode *sub = targetNode->subNodeHead;
    while (sub) {
        subNode *ref = sub->nextSubNode;
        free(sub);
        sub = ref;
    }
    targetNode->subNodeHead = NULL;
}

void deleteNode(int position, node *originNode) {
    while (position--) {
        originNode = originNode->nextNode;
    }
    originNode->nextNode->previousNode = originNode->previousNode;
    originNode->previousNode->nextNode = originNode->nextNode;
    if (originNode->subNodeHead) {
        (*fptr[7])(originNode);
    }
    free(originNode);
}

void deleteList(node *originNode) {
    node *ref = originNode->nextNode;
    while (ref != originNode) {
        node *aft = ref->nextNode;
        (*fptr[2])(1, originNode);
        ref = aft;
    }
    originNode->nextNode = originNode;
    originNode->previousNode = originNode;
    (*fptr[7])(originNode);
}

void printList(node *originNode) {
    int count = 0;
    printf("Printing clockwise:\n");
    node *iterator = originNode->nextNode;
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    (*fptr[4])(originNode);
    printf("\n   |\n   v\n");
    while (iterator != originNode) {
        count++;
        printf("[Pos %i:%i]", count, iterator->data);
        // printing subNodes
        (*fptr[4])(iterator);
        printf("\n   |\n   v\n");
        iterator = iterator->nextNode;
    }
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    (*fptr[4])(originNode);

    printf("\nPrinting counter-clockwise:\n");
    iterator = originNode->previousNode;
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    (*fptr[4])(originNode);
    printf("\n   |\n   v\n");
    while (iterator != originNode) {
        printf("[Pos %i:%i]", count, iterator->data);
        // printing subNodes
        (*fptr[4])(iterator);
        printf("\n   |\n   v\n");
        iterator = iterator->previousNode;
        count--;
    }
    printf("[Pos %i:%i]", 0, originNode->data);
    // printing subNodes
    (*fptr[4])(originNode);
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

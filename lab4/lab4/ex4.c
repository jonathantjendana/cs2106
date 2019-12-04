/*************************************
 * Lab 4 Exercise 4
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************/

// You can modify anything in this file. 
// Unless otherwise stated, a line of code being present in this template 
//  does not imply that it is correct/necessary! 
// You can also add any global or local variables you need (e.g. to implement your page replacement algorithm).

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "api.h"


typedef struct stackstruct {
    int capacity;
    int stackTop;
    int *array;
}STACK;

bool isEmpty(STACK* stack) {
    return stack->stackTop == -1;
}

bool isFull(STACK* stack) {
    return stack->stackTop == stack->capacity - 1;
}

int pop(STACK* stack) {
    int result;
    if (!isEmpty(stack)) {
        result = stack->array[stack->stackTop--];
    }
    return result;
}

void push(STACK* stack, int num) {
    if (!isFull(stack)) {
        stack->array[++(stack->stackTop)] = num;
    }
}

STACK* initStack(int size) {
    STACK * stack = malloc(sizeof(struct stackstruct));
    stack->array = malloc(size * sizeof(int));
    stack->capacity = size;
    stack->stackTop = -1;
    return stack;
}

void destroyStack(STACK* stack) {
    if (stack) {
        free(stack->array);
        free(stack);
    }
}

typedef struct QNode {
    int pageNum;
    struct QNode *next;
}QNode, *QNodePtr;

typedef struct {
    QNodePtr front;
    QNodePtr rear;
}Queue;

void initQueue(Queue* queue) {
    queue->front = queue->rear = (QNodePtr) malloc(sizeof(QNode));
    if (!queue->front) {
        printf("malloc error!\n");
        return;
    }
    queue->front->next = NULL;
    return;
}

void enqueue(Queue* queue, int pageNum) {
    QNodePtr node = (QNodePtr) malloc(sizeof(QNode));
    if (!node) {
        printf("malloc error!\n");
        return;
    }
    node->pageNum = pageNum;
    node->next = NULL;
    queue->rear->next = node;
    queue->rear = node;
}

int dequeue(Queue* queue) {
    QNodePtr node = NULL;
    int result;
    if (queue->front != queue->rear) {
        node = queue->front->next;
        result = node->pageNum;
        queue->front->next = node->next;
        if (queue->rear == node) {
            queue->rear = queue->front;
        }
        free(node);
    }
    return result;
}

void traversal(Queue queue) {
    int i = 1;
    QNode *q = queue.front->next;
    while (q) {
        printf("element at %d is %d\n", i, q->pageNum);
        q = q->next;
        ++i;
    }
}

void destroyqueue(Queue * queue) {
    while (queue->front) {
        queue->rear = queue->front->next;
        free(queue->front);
        queue->front = queue->rear;
    }
    return;
}

int const maxPageNum = 1<<PAGE_BITS;
int const maxFrameNum = 1<<FRAME_BITS;

void os_run(int initial_num_pages, page_table * pg_table) {
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset( & signals);
    sigaddset( & signals, SIGUSR1);
    sigaddset( & signals, SIGUSR2);
    bool *isMapped = calloc(maxPageNum, sizeof(*isMapped));
    // 0: not mapped, 1: mapped, not created, 2: mapped, created
    char *isCreated = calloc(maxPageNum, sizeof(*isCreated));
    // create the pages in the disk first, because every page must be backed by the disk for this lab
    for (int i = 0; i != initial_num_pages; ++i) {
        //disk_create(i);
        isCreated[i] = 1;
        isMapped[i] = true;
    }

    int *frames = malloc((maxFrameNum) * sizeof(*frames));
    for (int i = 0; i < maxFrameNum; ++i) {
        frames[i] = -1; // not occupied
    }
    int *invertedTable = malloc((maxFrameNum) * sizeof(*invertedTable));
    for (int i = 0; i < maxFrameNum; ++i) {
        invertedTable[i] = -1;
    }
    int nextVictim = 0;

    /*
     * either queue or stack will do the job.
     * unmmapped page will be reuse in the end.
     * get the not-mapped page from the front
     * add the unmapped page to the rear 
     */
    /*
    Queue queue;
    initQueue(&queue);
    for (int i = initial_num_pages; i < maxPageNum; ++i) {
        enqueue(&queue, i);
    }
    */

    // unmapped page will be reused first
    STACK *stack = initStack(maxPageNum);
    for (int i = maxPageNum - 1; i >= initial_num_pages; --i) {
        push(stack, i);
    }

    while (1) {
        union sigval reply_value;
        siginfo_t info;
        sigwaitinfo( & signals, & info);

        if (info.si_signo == SIGUSR1) {

            // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
            int const requested_page = info.si_value.sival_int;

            if (requested_page == -1) break;
            if (!isMapped[requested_page]) {
                reply_value.sival_int = 1;
            } else {
                /*
                printf("\tcurrent frame:\n\t| ");
                for (int i = 0; i < maxFrameNum; ++i) {
                    printf("%d | ", invertedTable[i]);
                }
                printf("\n");
                */
                // process the signal, and update the page table as necessary
                while (1) {
                    //printf("\tnow nextVictim pointing to frame %d\n", nextVictim);
                    int victimPage = invertedTable[nextVictim];
                    if (victimPage == -1) {
                        //printf("\tempty frame found at frame %d\n", nextVictim);
                        pg_table->entries[requested_page].valid = 1;
                        pg_table->entries[requested_page].frame_index = nextVictim;
                        invertedTable[nextVictim] = requested_page;
                        break;
                    } else if (!pg_table->entries[victimPage].referenced) {
                        //printf("\tgoing to replace page %d at frame %d\n", invertedTable[nextVictim], nextVictim);
                        if (pg_table->entries[victimPage].dirty) {
                            disk_write(nextVictim, victimPage);
                            pg_table->entries[victimPage].dirty = 0;
                        }
                        pg_table->entries[victimPage].valid = 0;
                        pg_table->entries[requested_page].valid = 1;
                        pg_table->entries[requested_page].frame_index = nextVictim;
                        invertedTable[nextVictim] = requested_page;
                        break;
                    } else {
                        //printf("\tpage %d at frame %d is referenced before, moving nextVictim to ", invertedTable[nextVictim], nextVictim);
                        pg_table->entries[victimPage].referenced = 0;
                        nextVictim = (nextVictim + 1) % (maxFrameNum);
                        //printf("%d\n", nextVictim);
                    }
                }
                if (isCreated[requested_page]&1) {
                    disk_create(requested_page);
                    isCreated[requested_page] = 2;
                }
                disk_read(nextVictim, requested_page);
                nextVictim = (nextVictim + 1) % (maxFrameNum);

                // tell the MMU that we are done updating the page table
                reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, set to 1 if the page is not mapped to the user process (i.e. segfault)
            }
        } else {
            int const tmp = info.si_value.sival_int;
            // mmap
            if (tmp == -1) {
                //int availablePage = dequeue(&queue);
                int availablePage = pop(stack);
                //printf("%d\n", availablePage);
                isMapped[availablePage] = true;
                //disk_create(availablePage);
                isCreated[availablePage] = 1;
                reply_value.sival_int = availablePage;
            // munmap
            } else {
                if (isMapped[tmp]) {
                    isMapped[tmp] = false;
                    push(stack, tmp);
                    //enqueue(&queue, tmp);
                    pg_table->entries[tmp].dirty = 0;
                    if (pg_table->entries[tmp].valid) {
                        pg_table->entries[tmp].valid = 0;
                        //printf("page %d at frame %d get munmapped\n", tmp, pg_table->entries[tmp].frame_index); 
                        invertedTable[pg_table->entries[tmp].frame_index] = -1;
                    }
                    if (isCreated[tmp] == 2) {
                        isCreated[tmp] = 0;
                        disk_delete(tmp);
                    }
                }
                reply_value.sival_int = 0;
            }
        }
        sigqueue(info.si_pid, SIGCONT, reply_value);
    }
    //destroyqueue(&queue);
    destroyStack(stack);
    free(isMapped);
    free(isCreated);
    free(invertedTable);
    free(frames);
}

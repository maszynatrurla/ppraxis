/**
 * @file pqueue.c
 * @author maszynatrurla
 * @brief Programming exercise: Priority queue.
 * 
 * Inspiration: http://programmingpraxis.com/2009/05/05/priority-queues/
 */

#include <stdio.h>
#include <stdlib.h>


#define OK      0
#define NOK     1

#define PAGE    100

typedef const char * Qdata;

typedef struct Node_t {
    Qdata           data;
    unsigned        priority;
    struct Node_t * left;
    struct Node_t * right;
} Node;

typedef Node * Queue;

typedef struct {
    int capacity;
    int size;
    Node ** elems;
} Stack;

int   pqueueInit(Queue * queue);
int   pqueueInsert(Queue * queue, Qdata data, unsigned priority);
Qdata pqueueFindFirst(Queue queue);
void  pqueueDeleteFirst(Queue * queue);
Queue pqueueMerge(Queue q0, Queue q1);

static Node * pop();
static int push(Node * n);

static Stack s_stack = {0, 0, 0};

static Node * pop() {
    if (s_stack.size) {
        return s_stack.elems[--s_stack.size];
    }
    return 0;
}

static int push(Node * n) {
    if (s_stack.size >= s_stack.capacity) {
        if (0 == s_stack.elems) {
            s_stack.elems = malloc(PAGE * sizeof(Node *));
        } else {
            printf("Out of memory1\n");
            return NOK;
            s_stack.elems = realloc(s_stack.elems, (s_stack.capacity + PAGE) * sizeof(Node *));
        }
        if (!s_stack.elems) {
            printf("Out of memory2\n");
            return NOK;
        }
        s_stack.capacity += PAGE;
    }
    s_stack.elems[s_stack.size++] = n;
    return OK;
}

int pqueueInit(Node ** root) {
    *root = 0;
    return OK;
}

int pqueueInsert(Queue * queue, Qdata data, unsigned priority) {
    Node * newNode = malloc(sizeof(Node));
    if (!newNode) {
        printf("Out of memory3\n");
        return NOK;
    }
    newNode->data = data;
    newNode->priority = priority;
    newNode->left = 0;
    newNode->right = 0;
    *queue = pqueueMerge(*queue, newNode);
    return OK;
}

Qdata pqueueFindFirst(Queue queue) {
    if (queue) {
        return queue->data;
    }
    return 0;
}

void pqueueDeleteFirst(Queue * queue) {
    if (*queue) {
        Queue left = (*queue)->left;
        Queue right = (*queue)->right;
        free(*queue);
        *queue = pqueueMerge(left, right);
    }
}

Queue pqueueMerge(Queue q0, Queue q1) {
    if (!q0) {
        return q1;
    }
    if (!q1) {
        return q0;
    }
    while (1) {
        if (q0->priority > q1->priority) {
            Queue t = q0;
            q0 = q1;
            q1 = t;
        }
        if (!q0->right) {
            Node * n;
            
            q0->right = q1;
            if (!q0->left || q0->left->priority > q0->right->priority) {
                Queue t = q0->left;
                q0->left = q0->right;
                q0->right = t;
            }
            
            while (0 != (n = pop())) {
                n->right = q0;
                q0 = n;
            }
            return q0;
        } else {
            if (OK != push(q0)) {
                return q0;
            }
            q0 = q0->right;
        }
    }
}

void pritri(Queue t, int tabs) {
    if (t) {
        int i;
        for (i = 0; i < tabs; ++i) {
            putchar(0x20);
            putchar(0x20);
        }
        printf("%d %s %p %p\n", t->priority, t->data, t->left, t->right);
        pritri(t->left, tabs + 1);
        pritri(t->right, tabs + 1);
    }
}


int main(int argc, char ** argv) {
    Queue queue;
    Qdata data;
    
    pqueueInit(&queue);
    
    pqueueInsert(&queue, "dziewiec", 9 );
    pqueueInsert(&queue, "pietnascie", 15);
    pqueueInsert(&queue, "dwadziescia trzy", 23);
    pqueueInsert(&queue, "czternascie", 14);
    pqueueInsert(&queue, "siedemnascie", 17);
    pqueueInsert(&queue, "osiem", 8 );
    pqueueInsert(&queue, "dwadziescia jeden", 21);
    pqueueInsert(&queue, "dwadziescia", 20);
    pqueueInsert(&queue, "dziewietnascie", 19);
    pqueueInsert(&queue, "dwadziescia dwa", 22);
    pqueueInsert(&queue, "dziesiec", 10);
    pqueueInsert(&queue, "siedem", 7 );
    pqueueInsert(&queue, "trzynascie", 13);
    pqueueInsert(&queue, "szesc", 6 );
    pqueueInsert(&queue, "trzy", 3 );
    pqueueInsert(&queue, "jeden", 1 );
    pqueueInsert(&queue, "jedenascie", 11);
    pqueueInsert(&queue, "dwanascie", 12);
    pqueueInsert(&queue, "zero", 0 );
    pqueueInsert(&queue, "cztery", 4 );
    pqueueInsert(&queue, "piec", 5 );
    pqueueInsert(&queue, "osiemnascie", 18);
    pqueueInsert(&queue, "dwa", 2 );
    pqueueInsert(&queue, "szesnascie", 16);
    
    pritri(queue, 0);
    
    while (0 != (data = pqueueFindFirst(queue))) {
        printf("%s\n", data);
        pqueueDeleteFirst(&queue);
    }
    
    return 0;
}

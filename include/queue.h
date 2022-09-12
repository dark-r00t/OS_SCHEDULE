#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

#include "processes.h"
#include <stdlib.h>

typedef struct node node;
struct node {
	node* head;
	node* tail;
	instruction_* process;
};

typedef struct queue {

	node* next;
	node* prev;
} queue;

queue* create_queue();
node* create_node(instruction_* process);
node* pop(queue* q);
void push(queue* q, node* n);
void push_rr(queue* q, node* n);

#endif

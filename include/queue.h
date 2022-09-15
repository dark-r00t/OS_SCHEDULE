#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

#include "processes.h"
#include <stdlib.h>

typedef struct node {
	struct node* head;
	struct node* tail;
	instruction_* process;
} node;

typedef struct queue {

	node* next;
	node* prev;
} queue;

queue* create_queue();
node* create_node(instruction_* process);
node* dequeue(queue* q);
void enqueue(queue* q, node* n);
void push_rr(queue* q, node* n);

#endif

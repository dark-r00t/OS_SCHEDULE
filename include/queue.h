#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

#include "processes.h"
#include <stdlib.h>

typedef struct queue {

	node* next;
	node* prev;
} queue;

queue* create_queue();
node* create_node(instruction_* process);
node* dequeue(queue* q);
void enqueue(queue* q, node* n);
void enqueue_arrival(queue* q, node* n);

#endif

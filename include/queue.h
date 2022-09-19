#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

#include "processes.h"

typedef struct queue {

	node* next;
	node* prev;

} queue;

node* 	create_node		(instruction_* process);
queue* 	create_queue	();
node* 	dequeue			(queue* q);
void 	enqueue			(queue* q, node* n);
void 	enqueue_arrival	(queue* q, node* n);

#endif

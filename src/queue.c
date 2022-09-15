#include "queue.h"

queue* create_queue() {

	queue* q = malloc(sizeof(queue));

	q->next = NULL;
	q->prev = NULL;

	return q;
}
	
node* create_node(instruction_* process) {

	node* n = malloc(sizeof(node));

	n->head=NULL;
	n->tail=NULL;
	n->process=process;

	return n;
} 

node* dequeue(queue* q) {

	if(!q || !q->next) return NULL;

	node* t = q->next; // node to dequeue
	q->next = q->next->head; // skip over next node

	t->head = NULL;// detaching from nodes
	t->tail = NULL;

	if(!q->next) q->prev = NULL;

	return t;
}

void enqueue(queue* q, node* n) {

	if(!q || !n) return;

	if(!q->next) {
		q->next = q->prev = n;
	} else {
		q->prev->head = n;
		q->prev = n;
	}

}

void enqueue_rr(queue* q, node* n) {
	// take a new node, push it into its correct spot.
	// essentially, sorting by arrival time

	if(!q || !n) return;

	node* t = q->next;
	while(t) {// stop on new lowest arrivval
		if(n->process->arrival < t->process->arrival) {
			break;
		} t = t->head;
	}

	if (!t) {
		enqueue(q, n);
		return;
	}

	if (t == q->next)
       	q->next = n;
	n->tail = t->tail;
	n->head = t;
	t->tail = n;
}

#include "queue.h"
#include <malloc.h>

queue* create_queue() {
	// creates a barebones queue; uninitialized
	queue* q = malloc(sizeof(queue));

	q->next = NULL;
	q->prev = NULL;

	return q;
}
	
node* create_node(instruction_* process) {
	// creates a barebones queue; initialized
	node* n = malloc(sizeof(node));

	n->head=NULL;
	n->process=process;

	return n;
} 

node* dequeue(queue* q) {

	if(!q || !q->next) return NULL;

	node* t = q->next; // node to dequeue
	q->next = q->next->head; // skip over next node

	t->head = NULL;// detaching from nodes

	if(!q->next) q->prev = NULL;

	return t;
}

void enqueue(queue* q, node* n) {

	if(!q || !n) return;

	if(!q->next) {// nothing in queue, add it and make a cycle
		q->next = q->prev = n;
	} else {// something in the queue, attach at the end
		q->prev->head = n;
		q->prev = n;
	}
}

void enqueue_arrival(queue* q, node* n) {
	// take a new node, push it into its correct spot.
	// essentially, sorting by arrival time

	if(!q || !n) return;

	node* t = q->next;
	while(t) {// stop on new lowest arrival
		if(n->process->arrival < t->process->arrival) break;
		if (t->head) if(n->process->arrival < t->head->process->arrival) {
			//sanity check
			n->head = t->head;
			t->head = n;
			return;
		}

		t = t->head;
	}

	if (!t) {
		enqueue(q, n);
		return;
	}

	if (t == q->next)// we never moved from the initial point...
       	q->next = n;
	n->head = t;
}

void enqueue_burst(queue* q, node* n) {

	if(!q || !n) return;

	node* t = q->next;
	while(t) {// stop on new lowest arrival
		if(n->process->burst_left < t->process->burst_left) break;
		if (t->head) if(n->process->burst_left < t->head->process->burst_left) {
			//sanity check
			n->head = t->head;
			t->head = n;
			return;
		}

		t = t->head;
	}

	if (!t) {
		enqueue(q, n);
		return;
	}

	if (t == q->next)// we never moved from the initial point...
       	q->next = n;// plop the node in as the first link
	n->head = t;
}
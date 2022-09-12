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

node* pop(queue* q) {

	if(!q || !q->next) return NULL;

	node* t = q->next;
	q->next = q->next->head;

	t->head = NULL;
	t->tail = NULL;

	if(q->next) q->next->tail = NULL;

	return t;
}

void push(queue* q, node* n) {

	if(!q || !n) return;

	if(!q->next) {
		q->next = n;
		n->head = NULL;
		n->tail = NULL;
		return;
	}

	if(!q->prev) {
		q->next->head = n;
		q->prev=n;
		n->head = NULL;
		n->tail = q->next;
		return;
	}

	q->prev->head = n;
	n->tail = q->prev;
	q->prev = n;
	n->head = NULL;
}

void push_rr(queue* q, node* n) {
	// take a new node, push it into its correct spot.
	// essentially, sorting by arrival time

	if(!q || !n) return;

	node* t = q->next;
	while(t) {
		if(n->process->burst < t->process->burst) 
			break;
		t = t->head;
	}

	if(!q->next) {
		q->next = n;
		n->head = NULL;
		n->tail = NULL;
		return;
	} 

	if (!t) {

		if(!q->prev) {
			q->prev = n;
			q->next->head = n;
			q->prev->tail = q->next;
		} else {
			q->prev->head = n;
			n->tail = q->prev;
			q->prev = n;
		}

		return; 
	} 

	if(t == q->next)
		q->next = n;

	n->tail = t->tail;
	n->head = t;
	t->tail = n;
}

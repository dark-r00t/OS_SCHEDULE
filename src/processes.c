#include "processes.h"
#include "queue.h"
#include "debug.h"

char* execute_process(instructions_* list) {

	if(list->use == RR) {
		return rr(list);
	} else if(list->use == FCFS) {
		return fcfs(list);
	} else if(list->use == SJF) {
		return sjf(list);
	}

	return NULL;
}

char* fcfs(instructions_* list) {

	return NULL;
}

char* sjf(instructions_* list) {

	return NULL;
}

char* rr(instructions_* list) {

	debug_print_list(list);

	queue* q = create_queue();

	for(int i = 0; i < list->processcount; i++) {
		node* current = create_node(list->id[i]);
		push_rr(q, current);
	}
	
	node* t = q->next;
	printf("%s", t->process->name);
	printf("%s", t->head->process->name);
	t = dequeue(q);
	printf("%s ", t->process->name);
	t = dequeue(q);
	printf("%s ", t->process->name);
	t = dequeue(q);
	if(!t) printf("!");

	return NULL;
} 

#include "processes.h"
#include "queue.h"
#include "debug.h"
#include <string.h>

void execute_process(instructions_* list) {

	FILE* output = fopen("processes.out", "w");

	// do first basic lines of printing 
	fprintf(output, "\t%d processes\n", list->processcount);
	fprintf(output, "\tUsing %s\n", 
		(list->use == RR) ? "Round-Robin" : (list->use == SJF) ? "Shortest Job First" : "First In First Out");
	if (list->use == RR) fprintf(output, "\tQuantum %d\n", list->quantum);

	if(list->use == RR) {
		rr(output, list);
	} else if(list->use == FCFS) {
		fcfs(output, list);
	} else if(list->use == SJF) {
		sjf(output, list);
	}

	fprintf(output, "\n\nClarifications\n");
	fclose(output);
}

void fcfs(FILE* output, instructions_* list) {

}

void sjf(FILE* output, instructions_* list) {

}

void rr(FILE* output, instructions_* list) {

	debug_print_list(list);

	queue* q = create_queue();
	queue* arrived = create_queue();

	// create new nodes and shove them into a queue based on arrival time
	for(int i = 0; i < list->processcount; i++) {
		push_rr(q, create_node(list->id[i]));
	}
	
	// generic scope variables
	int time = -1, current_time = 0, finished = 0;
	node* active_node = NULL;
	node* temp_node = NULL;

	while(++time != list->runfor) {// we will never run past the alloted time
		
		while(q->next && q->next->process->arrival == time) {
			// grab every node that arrives on time
			node* p;
			enqueue(arrived, (p = dequeue(q)));

			fprintf(output, "\tTime %d: %s arrived\n", time, p->process->name);
			p->process->time_u = time;
		}

		if(!active_node && arrived->next) {// TODO

		}
	}

	fprintf(output, "\tFinished at time %d\n\n", time);

	for(int i = 0; i < list->processcount; i++) {
		instruction_* p = list->id[i];
		fprintf(output, "\t%s wait %d turnaround %d\n", p->name, p->wait, (p->wait + p->burst));
	}

	free(q);
	free(arrived);
} 

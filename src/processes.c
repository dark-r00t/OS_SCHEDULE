#include "processes.h"
#include "queue.h"
#include "debug.h"
#include <string.h>

void execute_process(instructions_* list) {

	FILE* output = fopen("processes.out", "w");

	// do first basic lines of printing 
	fprintf(output, "%d processes\n", list->processcount);
	fprintf(output, "Using %s\n", 
		(list->use == RR) ? "Round-Robin" : (list->use == SJF) ? "Shortest Job First" : "First In First Out");
	if (list->use == RR) fprintf(output, "Quantum %d\n", list->quantum);

	if(list->use == RR) {
		rr(output, list);
	} else if(list->use == FCFS) {
		fcfs(output, list);
	} else if(list->use == SJF) {
		sjf(output, list);
	}

	//fprintf(output, "\n\nClarifications\n");
	fclose(output);
}

void fcfs(FILE* output, instructions_* list) {

}

void sjf(FILE* output, instructions_* list) {

}

void rr(FILE* output, instructions_* list) {

	// debug_print_list(list);

	queue* q = create_queue();
	queue* arrived = create_queue();

	// create new nodes and shove them into a queue based on arrival time
	for(int i = 0; i < list->processcount; i++) {
		enqueue_rr(q, create_node(list->id[i]));
	}
	
	// generic scope variables
	int time = -1, time_quantum = 0;
	node* active_node = NULL;

	while(++time != list->runfor) {// we will never run past the alloted time
		
		int finished = 0;

		while(q->next && q->next->process->arrival == time) {
			// grab every node that arrives on time
			node* p;
			enqueue(arrived, (p = dequeue(q)));

			p->process->time_u = time;

			fprintf(output, "Time %d: %s arrived\n", time, p->process->name);
		}

		if(!active_node && arrived->next) {
			active_node = arrived->next;
			rr_burst(output, time, active_node);
		}

		if(arrived->next && !arrived->next->process->burst_r) {
			//
			fprintf(output, "Time %d: %s finished\n", time, arrived->next->process->name);

			free(dequeue(arrived));
			finished = 1;

			if(arrived->next) {
				active_node = arrived->next;
				rr_burst(output, time, active_node);
			} else {
				active_node = NULL;
			}

			time_quantum = 0; // ! TODO
		}

		if(!finished && time_quantum == list->quantum) {

			arrived->next->process->time_u = time;
			enqueue(arrived, dequeue(arrived)); // shove the node to the back

			if(arrived->next) {
				active_node = arrived->next;
				rr_burst(output, time, active_node);
			} else {
				active_node = NULL;
			}

			time_quantum = 0; // ! TODO
		}

		if(arrived->next) {
			arrived->next->process->burst_r--;
			time_quantum++; // ! TODO
		}

		if(!arrived->next) fprintf(output, "Time %d: Idle\n", time);
	}

	if(active_node && !active_node->process->burst_r) {
		instruction_* p = active_node->process;
		fprintf(output, "Time %d: %s finished\n", time, p->name);
		free(active_node);
	}

	fprintf(output, "Finished at time %d\n\n", time);

	for(int i = 0; i < list->processcount; i++) {
		instruction_* p = list->id[i];
		fprintf(output, "%s wait %d turnaround %d\n", p->name, p->wait, (p->wait + p->burst));
	}

	free(q);
	free(arrived);
} 

void rr_burst (FILE* output, int time, node* active_node) {

	instruction_* p = active_node->process;
	p->wait += time - p->time_u;

	fprintf(output, "Time %d: %s selected (burst %d)\n", time, p->name, p->burst_r);
}
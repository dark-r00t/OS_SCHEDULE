#include "processes.h"
#include "queue.h"
#include "debug.h"
#include "defs.h"
#include <string.h>

void execute_process(instructions_* list) {

	FILE* output = fopen("processes.out", "w");

	// do first basic lines of printing 
	fprintf(output, "%d processes\n", list->processcount);
	fprintf(output, "Using %s\n", 
		(list->use == RR) ? "Round-Robin" : (list->use == SJF) ? "Shortest Job First" : "First In First Out");
	
	if (list->use == RR) {
		fprintf(output, "Quantum %d\n\n", list->quantum);
	} else {
		fprintf(output, "\n");
	}

	if(list->use == RR) {
		rr(output, list);
	} else if(list->use == FCFS) {
		fcfs(output, list);
	} else if(list->use == SJF) {
		sjf(output, list);
	}

	fclose(output);
}

void fcfs(FILE* output, instructions_* list) {

	// LOG; debug_print_list(list);

	queue* unused = create_queue();
	queue* arrived = create_queue();

	// create new nodes and shove them into a queue based on arrival time
	for(int i = 0; i < list->processcount; i++) {
		enqueue_arrival(unused, create_node(list->id[i]));
	}


}

void sjf(FILE* output, instructions_* list) {

	// LOG; debug_print_list(list);
}

void rr(FILE* output, instructions_* list) {

	// LOG; debug_print_list(list);

	queue* unused = create_queue();
	queue* arrived = create_queue();

	// create new nodes and shove them into a queue based on arrival time
	for(int i = 0; i < list->processcount; i++) {
		enqueue_arrival(unused, create_node(list->id[i]));
	}
	
	// generic scope variables
	int time = -1, time_quantum = 0;
	node* active_node = NULL;

	while(++time != list->runfor) {// we will never run past the alloted time
		
		int finished = 0;

		while(unused->next && unused->next->process->arrival == time) {
			// grab every node that arrives on time, otherwise leavve it in 'unused'
			node* p;
			enqueue(arrived, (p = dequeue(unused)));

			p->process->time_used = time;

			fprintf(output, "Time %d: %s arrived\n", time, p->process->name);
		}

		if(!active_node && arrived->next) {
			active_node = arrived->next;
			rr_burst(output, time, active_node);
		}

		if(arrived->next && !arrived->next->process->burst_left) {
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

			time_quantum = 0;
		}

		if(!finished && time_quantum == list->quantum) {

			arrived->next->process->time_used = time;
			enqueue(arrived, dequeue(arrived)); // shove the node to the back

			if(arrived->next) {
				active_node = arrived->next;
				rr_burst(output, time, active_node);
			} else {
				active_node = NULL;
			}

			time_quantum = 0;
		}

		if(arrived->next) arrived->next->process->burst_left--;
		else fprintf(output, "Time %d: IDLE\n", time);

		time_quantum++;
	}

	if(active_node && !active_node->process->burst_left) {
		instruction_* p = active_node->process;
		fprintf(output, "Time %d: %s finished\n", time, p->name);
		free(active_node);
	}

	fprintf(output, "Finished at time %d\n\n", time);

	for(int i = 0; i < list->processcount; i++) {
		// ! TODO turnaround data sus, also only case of p->burst, maybe remove p->burst_left and just decrement og
		instruction_* p = list->id[i]; 
		fprintf(output, "%s wait %d turnaround %d\n", p->name, p->wait, (p->wait + p->burst));
	}

	// LOG; debug_print_list(list);

	free(unused);
	free(arrived);
} 

void rr_burst(FILE* output, int time, node* active_node) {

	instruction_* p = active_node->process;
	p->wait += time - p->time_used;

	fprintf(output, "Time %d: %s selected (burst %d)\n", time, p->name, p->burst_left);
}
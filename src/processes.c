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
		(list->use == RR) ? "Round-Robin" : (list->use == SJF) ? "Shortest Job First (Pre)" : "First Come First Served");
	
	if (list->use == RR) {
		fprintf(output, "Quantum %d\n\n", list->quantum);
	} else {
		fprintf(output, "\n");
	}

	if(list->use == RR) {
		schedule_arrival(output, list, RR);
	} else if(list->use == FCFS) {
		schedule_arrival(output, list, FCFS);
	} else if(list->use == SJF) {
		sjf(output, list);
	}

	fclose(output);
}

void schedule_arrival(FILE* output, instructions_* list, int type) {

	// LOG; debug_print_list(list);

	if(type == SJF) return; // this should never happen...

	queue* unused = create_queue();
	queue* arrived = create_queue();

	// generic scope variables
	int time = -1, time_quantum = 0;
	node* active_node = NULL;

	// create new nodes and shove them into a queue based on arrival time
	for(int i = 0; i < list->processcount; i++) {
		enqueue_arrival(unused, create_node(list->id[i]));
	}

	while(++time != list->runfor) {// we will never run past the alloted time
		
		int finished = 0;

		while(unused->next && unused->next->process->arrival == time) {
			// grab every node that arrives on the current time, otherwise leave it in 'unused'
			node* p;
			enqueue(arrived, (p = dequeue(unused)));

			p->process->time_used = time;

			fprintf(output, "Time %d: %s arrived\n", time, p->process->name);
		}

		if(!active_node && arrived->next) {
			// no active node, but a node available
			active_node = arrived->next;
			burst(output, time, active_node);
		}

		if(active_node && !active_node->process->burst_left) {
			// no burst left on active node
			fprintf(output, "Time %d: %s finished\n", time, arrived->next->process->name);

			free(dequeue(arrived));// detach active node (active_node == arrived->next), then free it
			finished = 1;

			if(arrived->next) {// more processes to handle
				active_node = arrived->next;
				burst(output, time, active_node);
			} else {// that's all she wrote
				active_node = NULL;
			}

			if (type == RR) time_quantum = 0;
		}

		if (type == RR) {

			if(!finished && time_quantum == list->quantum) {

				active_node->process->time_used = time;
				enqueue(arrived, dequeue(arrived)); // shove the active_node to the back

				if(active_node) {
					active_node = arrived->next;
					burst(output, time, active_node);
				}

				time_quantum = 0;
			}
		}

		if(active_node) active_node->process->burst_left--;
		else fprintf(output, "Time %d: IDLE\n", time);// idle until time runfor time has been reached

		if (type == RR) time_quantum++;
	}

	if(active_node && !active_node->process->burst_left) {
		// the last active node finished successfully
		instruction_* p = active_node->process;
		fprintf(output, "Time %d: %s finished\n", time, p->name);
	} else if (active_node && active_node->process->burst_left) {
		// the last active node ran out of time
		instruction_* p = active_node->process;
		fprintf(output, "%s wait %d did not complete\n", p->name, p->wait);
	}

	if(active_node) free(active_node);

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

void sjf(FILE* output, instructions_* list) {

	// LOG; debug_print_list(list);
}

void burst(FILE* output, int time, node* active_node) {

	instruction_* p = active_node->process;
	p->wait += time - p->time_used;

	fprintf(output, "Time %d: %s selected (burst %d)\n", time, p->name, p->burst_left);
}
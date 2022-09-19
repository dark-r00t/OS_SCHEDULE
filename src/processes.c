#include "processes.h"
#include "queue.h"
#include "defs.h"

#include <string.h>
#include <malloc.h>

void execute_process(instructions_* list) {

	FILE* output = fopen("processes.out", "w");

	// do first basic lines of printing 
	fprintf(output, "%d processes\n", list->processcount);
	fprintf(output, "Using %s\n", 
		(list->use == FCFS) ? "First Come First Served" : (list->use == RR) ? "Round-Robin" : "Shortest Job First (Pre)");
	
	if (list->use == RR) {// RR is the only case where time quantum is necessary
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

void sjf(FILE* output, instructions_* list) {

	schedule_burst(output, list, SJF);
}

void rr(FILE* output, instructions_* list) {

	schedule_arrival(output, list, RR);
}

void fcfs(FILE* output, instructions_* list) {

	schedule_arrival(output, list, FCFS);
}

void burst(FILE* output, int time, instruction_* p) {

	p->wait += time - p->time_used;
	fprintf(output, "Time %d: %s selected (burst %d)\n", time, p->name, p->burst_left);
}

void schedule_arrival(FILE* output, instructions_* list, int type) {

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

		if(!active_node) if(arrived->next) {
			// no active node, but a node available
			active_node = arrived->next;
			burst(output, time, active_node->process);
		}

		if(active_node) if(!active_node->process->burst_left) {
			// no burst left on active node
			fprintf(output, "Time %d: %s finished\n", time, arrived->next->process->name);

			free(dequeue(arrived));// detach active node (active_node == arrived->next), then free it
			finished = 1;// ONLY APPLICABLE FOR ROUND ROBIN

			if(arrived->next) {// more processes to handle
				active_node = arrived->next;
				burst(output, time, active_node->process);
			} else {// that's all she wrote
				active_node = NULL;
			}

			if (type == RR) time_quantum = 0;
		}

		if (type == RR) {

			if(!finished) if(time_quantum == list->quantum) {

				active_node->process->time_used = time;
				enqueue(arrived, dequeue(arrived)); // shove the active_node to the back

				if(active_node) {
					active_node = arrived->next;
					burst(output, time, active_node->process);
				}

				time_quantum = 0;
			}
		}

		if(active_node) active_node->process->burst_left--;
		else fprintf(output, "Time %d: IDLE\n", time);// idle until time runfor time has been reached

		if (type == RR) time_quantum++;
	}

	if(active_node) if(!active_node->process->burst_left) {
		// the last active node finished successfully
		instruction_* p = active_node->process;
		fprintf(output, "Time %d: %s finished\n", time, p->name);
	} else if (active_node) if(active_node->process->burst_left) {
		// the last active node ran out of time
		instruction_* p = active_node->process;
		fprintf(output, "%s wait %d did not complete\n", p->name, p->wait);
	}

	if(active_node) free(active_node);

	fprintf(output, "Finished at time %d\n\n", time);

	for(int i = 0; i < list->processcount; i++) {

		instruction_* p = list->id[i]; 
		fprintf(output, "%s wait %d turnaround %d\n", p->name, p->wait, (p->wait + p->burst));
		// printf("%d: %d %d %d %d %d\n", i+1, list->runfor, p->time_used, p->arrival, p->wait, p->burst);
	}

	free(unused);
	free(arrived);
}

void schedule_burst(FILE* output, instructions_* list, int type) {

	queue* unused = create_queue();
	queue* arrived = create_queue();

	// generic scope variables
	int time = -1;
	node* active_node = NULL;

	// create new nodes and shove them into a queue based on arrival time
	for(int i = 0; i < list->processcount; i++) {
		enqueue_arrival(unused, create_node(list->id[i]));
	}

	/* for(int i = 0; i < list->processcount; i++) {
		enqueue_burst(arrived, create_node(list->id[i]));
	} printf("\n"); 

	active_node = arrived->next;
	while(active_node) {
		printf("%d ", active_node->process->burst_left);
		active_node = active_node->head;
	} */

	while(++time != list->runfor) {// we will never run past the alloted time

		while(unused->next && unused->next->process->arrival == time) {
			// grab every node that arrives on the current time, otherwise leave it in 'unused'
			node* p;
			enqueue_burst(arrived, (p = dequeue(unused)));
			p->process->time_used = time;

			fprintf(output, "Time %d: %s arrived\n", time, p->process->name);
		}

		if(!active_node && arrived->next) {
			// no active node, but a node available
			active_node = arrived->next;
			burst(output, time, active_node->process);
		}

		if(active_node && !active_node->process->burst_left) {
			// no burst left on active node
			fprintf(output, "Time %d: %s finished\n", time, active_node->process->name);

			dequeue(arrived);

		} else if (arrived->next && active_node->process->burst_left > arrived->next->process->burst_left) {// ! TODO
			active_node->process->time_used = time;
			node* p = active_node;
			active_node = dequeue(arrived);
			active_node->process->wait += time - active_node->process->time_used;
			enqueue_burst(arrived, p);
			fprintf(output, "Time %d: %s selected (burst %d)\n", time, active_node->process->name, active_node->process->burst_left);
		}

		if(active_node) active_node->process->burst_left--;
		else fprintf(output, "Time %d: IDLE\n", time);// idle until time runfor time has been reached
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

		instruction_* p = list->id[i]; 
		fprintf(output, "%s wait %d turnaround %d\n", p->name, p->wait, (p->wait + p->burst));
		// printf("%d: %d %d %d %d %d\n", i+1, list->runfor, p->time_used, p->arrival, p->wait, p->burst);
	}

	free(unused);
	free(arrived);
}
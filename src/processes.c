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
		schedule(output, list, RR);
	} else if(list->use == FCFS) {
		schedule(output, list, FCFS);
	} else if(list->use == SJF) {
		schedule(output, list, SJF);
	}

	fclose(output);
}

void burst(FILE* output, int time, instruction_* p) {

	p->wait += time - p->time_used;
	fprintf(output, "Time %d: %s selected (burst %d)\n", time, p->name, p->burst_left);
}

void schedule(FILE* output, instructions_* list, int type) {

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
			if (type == SJF) {// SJF requires an enqueue by burst length
				enqueue_burst(arrived, (p = dequeue(unused)));
			} else {
				enqueue(arrived, (p = dequeue(unused)));
			}

			p->process->time_used = time;// immediately record when the process was last used...

			fprintf(output, "Time %d: %s arrived\n", time, p->process->name);
		}

		if(!active_node && arrived->next) {
			// no active node, but a node is available
			active_node = dequeue(arrived);
			burst(output, time, active_node->process);
		} 
		
		if (type == SJF && active_node && arrived->next) {
			if (active_node->process->burst_left > arrived->next->process->burst_left) {
				// theres a process in the arrived queue with a shorter burst time
				active_node->process->time_used = time;// record this current nodes last use
				
				enqueue_burst(arrived, active_node);// re-add this node to the queue (based on burst length of course)
				active_node = dequeue(arrived);//      update our new active_node with the one w/ shorter burst time
				
				burst(output, time, active_node->process);
			}
		}

		if(active_node && !active_node->process->burst_left) {
			// no burst left on active node
			fprintf(output, "Time %d: %s finished\n", time, active_node->process->name);

			free(active_node);// this node isn't needed anymore; 
							  // note: the process data is still malloc'd inside of "instructions_* list"

			if(arrived->next) {// more processes to handle
				active_node = dequeue(arrived);
				burst(output, time, active_node->process);
			} else {// that's all she wrote
				active_node = NULL;
			}

			if (type == RR) time_quantum = 0;
			if (type == RR) finished = 1;
		}

		if (type == RR) {

			if(!finished && time_quantum == list->quantum) {
				// in RR we always swap nodes "fairly" based on the time quantum
				if(active_node)  {
					active_node->process->time_used = time;// record this nodes last use time
					enqueue(arrived, active_node);// put active_node back into the queue
					active_node = dequeue(arrived);// time to give this node its' fair turn
					burst(output, time, active_node->process);
				}

				time_quantum = 0;// reset time quantum since a new node (theoretically) was chosen
			}
		}

		if(active_node) active_node->process->burst_left--;// decrement from the burst pool after e/ cycle
		else fprintf(output, "Time %d: IDLE\n", time);// idle until runfor time has been reached

		if (type == RR) time_quantum++; // RR requires us to keep track of how long a process has run, check line 110
	}

	if(active_node && !active_node->process->burst_left) {// edge case
		// the last active node finished successfully
		instruction_* p = active_node->process;
		fprintf(output, "Time %d: %s finished\n", time, p->name);
		free(active_node);
	} 
	
	while (arrived->next) {
		//cycle through all arrived nodes until theres none left
		active_node = dequeue(arrived);
		instruction_* p = active_node->process;
		fprintf(output, "%s wait %d did not complete\n", p->name, p->wait);
		free(active_node);
	}

	while (unused->next) {
		//cycle through all unused nodes until theres none left
		active_node = dequeue(unused);
		instruction_* p = active_node->process;
		fprintf(output, "%s could not be scheduled\n", p->name);
		free(active_node);
	}

	fprintf(output, "Finished at time %d\n\n", time);// record when we finally finished

	for(int i = 0; i < list->processcount; i++) {
		// for each process, state the wait and turnaround time
		instruction_* p = list->id[i]; 
		fprintf(output, "%s wait %d turnaround %d\n", p->name, p->wait, (p->wait + p->burst));
		free(p);
	}

	free(list);
}
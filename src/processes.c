#include "processes.h"
#include "queue.h"
#include "debug.h"
#include <string.h>

void execute_process(instructions_* list) {

	FILE* output = fopen("processes.out", "w");

	// create space for use type and copy name into a string
	char* out_tmp = (char*) malloc(sizeof(char) * 20);
	(list->use == RR) ? strcpy(out_tmp, "Round-Robin") : 
		(list->use == SJF) ? strcpy(out_tmp, "Shortest Job First") : 
		strcpy(out_tmp, "First In First Out");

	// do first basic lines of printing 
	fprintf(output, "\t%d processes\n", list->processcount);
	fprintf(output, "\tUsing %s\n", out_tmp);
	if (list->use == RR) fprintf(output, "\tQuantum %d\n", list->quantum);

	if(list->use == RR) {
		rr(output, list);
	} else if(list->use == FCFS) {
		fcfs(output, list);
	} else if(list->use == SJF) {
		sjf(output, list);
	}


	free(out_tmp);
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
		node* current_node = create_node(list->id[i]);
		push_rr(q, current_node);
	}
	
	// generic scope variables
	int time = 0, current_time = 0, finished = 0;
	node* active_node = NULL;
	node* temp_node = NULL;

	while(time != list->runfor) {// we will never run past the alloted time
		
		while(q->next && q->next->process->arrival == time) {
			// grab every node that arrives on time
			node* p;
			enqueue(arrived, (p = dequeue(q)));

			fprintf(output, "\tTime %d: %s arrived\n", time, p->process->name);
			p->process->time_u = time;
		}

		if(!active_node && arrived->next) {// TODO

		}

		time++;
	}

	fprintf(output, "\tFinished at time %d\n\n", time);

	for(int i = 0; i < list->processcount; i++) {
		instruction_* p = list->id[i];
		fprintf(output, "\t%s wait %d turnaround %d\n", p->name, p->wait, (p->wait + p->burst));
	}

	free(q);
	free(arrived);
} 

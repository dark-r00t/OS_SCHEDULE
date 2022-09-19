#ifndef PROCESSES_INCLUDED 
#define PROCESSES_INCLUDED

#include <stdio.h>

typedef struct instruction_ {

	char* name;		// process name
	int arrival;	// process arrival time
	int burst;		// process burst
	int burst_left;	// process burst left
	int wait;		// process wait time
	int time_used;	// process last time used

} instruction_;


typedef struct instructions_ {

	int processcount;	// number of processes;			id:1
	int runfor; 		// total time to run program;	id:2
	int use;			// type flag (FCFS, SJF, RR);	id:3
	int quantum;

	instruction_** id;	// data for e/ process;			id:4

} instructions_;

typedef struct node {

	struct node* head;		// way to traverse the list
	instruction_* process;	// copy of the original instruction_ data

} node;

void execute_process	(instructions_* list);

void fcfs				(FILE* output, instructions_* list);
void rr					(FILE* output, instructions_* list);
void sjf				(FILE* output, instructions_* list);

void burst				(FILE* output, int time, instruction_* p);
void schedule_arrival	(FILE* output, instructions_* list, int type);

#endif
#ifndef PROCESSES_INCLUDED 
#define PROCESSES_INCLUDED

#include <stddef.h>
#include <stdio.h>
#include <printf.h>
#include "types.h"

typedef struct instruction_ {

	char* name;
	int arrival;

	int burst;
	int burst_l;

} instruction_;


typedef struct instructions_ {

	int processcount; // 1
	int runfor; // 2
	int use; // 3
	int quantum;// 3.5

	instruction_** id; //4

} instructions_;

char* execute_process(instructions_* list);
char* fcfs(instructions_* list);
char* sjf(instructions_* list);
char* rr(instructions_* list);

#endif

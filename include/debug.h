#ifndef DEBUG_INCLUDED 
#define DEBUG_INCLUDED

#include <stdio.h>
#include "defs.h"
#include "processes.h"

/*

typedef struct instruction_ {

	char* name;
	int arrival;
	int burst;
	int burst_left;
	int wait;
	int time_used;

} instruction_;

*/

void debug_print_list(instructions_* list) {

	printf("\nDEBUG::\n\n");

	int p = list->processcount;
	int rf = list->runfor;
	int u = list->use;

	printf("\tProcess Count:\t%d\n\tRun For:\t%d\n\tUse:\t\t%x\n", p, rf, u);

	if(list->quantum) printf("\tQuantum:\t%d\n", list->quantum);
	printf("\t-------------------");

	for(int i = 0; i < list->processcount; i++) {
		instruction_* p = list->id[i];
		printf("\n\t%sName:\t\t%s%s\n\t%sArrival:\t%d%s\n\t%sBurst:\t\t%d%s\n\t%sBurst Left:\t%d%s\n\t%sWait:\t\t%d%s\n\t%sTime Used:\t%d%s\n\n", 
			GREEN, p->name, CLEAR,
			CYAN, p->arrival, CLEAR,
			CYAN, p->burst, CLEAR,
			RED, p->burst_left, CLEAR,
			CYAN, p->wait, CLEAR,
			RED, p->time_used, CLEAR
		);
	}
	printf("::GUBED\n\n");
} 

#endif

#ifndef DEBUG_INCLUDED 
#define DEBUG_INCLUDED

#include <stdio.h>
#include "processes.h"

void debug_print_list(instructions_* list) {

	printf("\nDEBUG::\n");

	int p = list->processcount;
	int rf = list->runfor;
	int u = list->use;

	printf("p:\t%d\nrf:\t%d\nu:\t%x\n", p, rf, u);

	if(list->quantum) printf("q:\t%d\n", list->quantum);

	for(int i = 0; i < list->processcount; i++) {

		printf("\t%s, %d, %d, %d\n", list->id[i]->name, list->id[i]->arrival, list->id[i]->burst, list->id[i]->burst_r);
	}
	printf("::GUBED\n\n");
} 

#endif

#include "processes.h"
#include "parse.h"
#include <stdlib.h>

int main(void) {

	char* process = get_process();
	
	instructions_* list = parse_process(process);
	
	execute_process(list);
	
	free(process);
	return 0;
} 

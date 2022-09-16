#include "file.h"
#include "processes.h"
#include "defs.h"
#include "leak_detector_c.h"
#include <stdlib.h>

int main(int argc, char** argv) {

	atexit(report_mem_leak);

	// Please note, this is mostly for testing. 
	// By default the program will just run "processes.in" 
	// from the root directory.
	char* optional_in = NULL;
	if(argc == 2) {
		optional_in = argv[1];
		printf("\nNOTE: Running file %s\"%s\"%s\n", GREEN, optional_in, CLEAR);
	}
	
	char* process = get_process(optional_in);
	instructions_* list = parse_process(process);
	execute_process(list);
	
	free(process);
	return 0;
} 

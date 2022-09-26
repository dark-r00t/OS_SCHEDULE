#include "file.h"
#include "processes.h"
#include "defs.h"
#include <stdlib.h>

int main(int argc, char** argv) {
	// Please note, this is mostly for testing. 
	// By default the program will just run "processes.in" 
	// from the root directory.
	char* optional_in = NULL;
	if(argc == 2) {
		optional_in = argv[1];
		printf("\nNOTE: Running file %s\"%s\"%s\n", 
			GREEN, optional_in, CLEAR
		);
	} else if (argc > 2) return 1;
	
	char* process = get_process(optional_in);
	if(!process) return 1;

	instructions_* list = parse_process(process);
	if(!list) return 1;

	execute_process(list);
	
	free(process);
	return 0;
} 

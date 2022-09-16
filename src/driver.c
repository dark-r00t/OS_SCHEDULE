#include "file.h"
#include "processes.h"
#include "leak_detector_c.h"
#include <stdlib.h>

int main(void) {

	atexit(report_mem_leak);
	
	char* process = get_process();
	instructions_* list = parse_process(process);
	execute_process(list);
	
	free(process);
	return 0;
} 

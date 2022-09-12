#include "processes.h"
#include "parse.h"
#include <stdlib.h>

int main(void) {

	char* process = get_process();
	
	instructions_* list = parse_process(process);
	
	char* output = execute_process(list);

	FILE* fp = fopen("p.out", "w");
	fclose(fp);
	
	free(process);
	return 0;
} 

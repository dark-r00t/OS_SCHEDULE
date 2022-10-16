#include "file.h"
#include "defs.h"

#include <stdlib.h>
#include <string.h>

char* get_process(char* optional_in) {

	// For testing, more information in "driver.c"
	FILE* fp;
	if(!optional_in) fp = fopen("processes.in", "r");
	else 			 fp = fopen(optional_in, "r");

	if(!fp) {
		printf("ERROR READING FILE!\n");
		if (optional_in) printf("%s%s%s caused an error.\nPerhaps, try retyping and varifying your input file name and path.\n", 
						 	RED, optional_in, CLEAR);
		else 			 printf("Ensure %sprocesses.in%s is in the root directory and try again.\n", 
							RED, CLEAR);
		return NULL;
	}
	
	// GET FILE AND FILE LENGTH
	fseek(fp, 0, SEEK_END);
	unsigned long fp_size = ftell(fp);
	rewind(fp);

	// CREATE SPACE FOR CHARACTERS
	char* process_tmp = (char*) malloc(fp_size);
	char* process = (char*) malloc(fp_size);
	fread(process_tmp, fp_size, 1, fp);
	fclose(fp);

	char c;
	int loc = 0; 
	for(unsigned long p = 0; p < fp_size; p++) {

		if(process_tmp[p] == ' ' && process_tmp[p+1] == ' ') {
			// GET RID OF ALL EXTRA WHITE SPACE
			while((c = process_tmp[++p]) == ' ');
		}

		if(process_tmp[p] == '#'){
			// GET RID OF COMMENTS
			while ((c = process_tmp[++p]) != '\n');
			process[loc++] = ' ';// seperator space, for later use with strtok()
			continue; // continue because we dont know if the line is one entire comment
		}

		//STORE VALUE (guranteed to be useful)
		if(process_tmp[p] == '\n' || process_tmp[p] == ' ') {
			process[loc] = ' ';
			loc++;
		} else {
			process[loc++] = process_tmp[p];
		}
	}

	free(process_tmp);
	return process;
}

int get_process_id(char* token) {

	if(!strcmp(token, "processcount")) { return 1; } 
	if(!strcmp(token, "runfor"))	   { return 2; } 
	if(!strcmp(token, "use"))		   { return 3; } 
	if(!strcmp(token, "process"))	   { return 4; } 
										 return 0;
} 

instructions_* parse_process(char* input) {

	// see 'processes.h' for more detail on 'instructions_'
	instructions_* list = (instructions_*) malloc(sizeof(instructions_));

	// tokenizing "chunks" up the original string, we want to avoid that in our function
	char* process = (char*) malloc(sizeof(char) * strlen(input) + 1);
	strcpy(process, input);

	char* token = strtok(process, " ");// tokenize by white space
	while(token) {

		int tmp = get_process_id(token);// notice the usse of this in the switch case
		token = strtok(NULL, " ");// step ahead one token

		switch(tmp) {

			case 1:{// processcount
				list->processcount = atoi(token);

				// malloc space for each instruction
				list->id = (instruction_**) malloc(sizeof(instruction_*) * list->processcount);
				
				for(int i = 0; i < list->processcount; i++) {
					// for each instruction create space for struct and the name
					list->id[i] = (instruction_*) malloc(sizeof(instruction_));
					list->id[i]->name = (char*) malloc(sizeof(char) * 3);
				}

				break;
			}

			case 2:{// runfor
				list->runfor = atoi(token);
				break;
			}

			case 3:{// use
				if (strcmp(token, "rr") == 0)  {
					strtok(NULL, " ");	
					token = strtok(NULL, " ");

					list->use = RR;
					list->quantum = atoi(token);

				} else if (strcmp(token, "fcfs") == 0) {
					list->use = FCFS;
					list->quantum = 0;

				} else if(strcmp(token, "sjf") == 0) {
					list->use = SJF;
					list->quantum = 0;

				} break;
			}

			case 4:{// name/arrival/burst are always paired

				token = strtok(NULL, " ");
				for(int i = 0; i < list->processcount; i++) {
					strcpy(list->id[i]->name, token);
					strtok(NULL, " ");
					token = strtok(NULL, " ");

					list->id[i]->arrival = atoi(token);
					strtok(NULL, " ");	
					token = strtok(NULL, " ");

					list->id[i]->burst = list->id[i]->burst_left = atoi(token);
					strtok(NULL, " ");	
					token = strtok(NULL, " ");
					token = strtok(NULL, " ");
				}

				break;
			}

			default: break;
		}

		token = strtok(NULL, " ");// move to next token, while loop then validates it
	}

	free(process);
	return list;
}

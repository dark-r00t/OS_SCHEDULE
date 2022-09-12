#include "parse.h"

char* get_process() {

	// GET FILE AND FILE LENGTH
	FILE* fp = fopen("processes.in", "r");
	fseek(fp, 0, SEEK_END);
	unsigned long fp_size = ftell(fp);
	rewind(fp);

	// CREATE SPACE FOR CHARACTERS
	char* process_tmp = (char*) malloc(fp_size+1);
	char* process = (char*) malloc(fp_size);
	fread(process_tmp, fp_size, 1, fp);
	fclose(fp);

	char c;
	int loc = 0;
	for(unsigned long p = 0; p < fp_size; p++) {

		if(process_tmp[p] == ' ' && process_tmp[p+1] == ' ') {
			// GET RID OF ALL EXTRA WHITE SPACE
			while((c = process_tmp[p++]) == ' ');
			p-=1;
		}
		if(process_tmp[p] == '#'){
			// GET RID OF COMMENTS
			while ((c = process_tmp[p++]) != '\n');
			process[loc++] = ' ';
			continue;
		}

		//STORE VALUE
		process[loc++] = process_tmp[p];
	}

	free(process_tmp);

	return process;
}

instructions_* parse_process(char* input) {

	instructions_* list = (instructions_*) malloc(sizeof(instructions_));

	int tmp;
	int l = strlen(input);

	// tokenizing messes up the original string, we want to avoid that.
	char* process = (char*) malloc(sizeof(char) * l + 1);
	strcpy(process, input);

	char* token = strtok(process, " ");
	while(token) {

		tmp = get_process_id(token);
		token = strtok(NULL, " ");// step ahead one token

		switch(tmp) {
			case 1:{// processcount
				list->processcount = atoi(token);

				list->id = (instruction_**) malloc(sizeof(instruction_*) * list->processcount);
				for(int i = 0; i < list->processcount; i++) {
					list->id[i] = (instruction_*) malloc(sizeof(instruction_));
					list->id[i]->name = malloc(sizeof(char) * 3);
				}

				break;
			}
			case 2:{// runfor
				list->runfor = atoi(token);
				break;
			}
			case 3:{// use
				if (strcmp(token, "rr") == 0)  {
					list->use = RR;

					token = strtok(NULL, " ");
					if(strcmp(token, "quantum")) break; // this should never happen
					token = strtok(NULL, " ");

					list->quantum = atoi(token);

				} else if (strcmp(token, "fcfs") == 0) {
					list->use = FCFS;

				} else if(strcmp(token, "sjf") == 0) {
					list->use = SJF;

				} break;
			}
			case 4:{// name

				for(int i = 0; i < list->processcount; i++) {
					token = strtok(NULL, " "); 
					strcpy(list->id[i]->name, token);

					token = strtok(NULL, " ");
					token = strtok(NULL, " ");
					list->id[i]->arrival = atoi(token);

					token = strtok(NULL, " ");
					token = strtok(NULL, " ");
					list->id[i]->burst = atoi(token);
					token = strtok(NULL, " ");
				}

				break;
			}
			default: break;
		}

		token = strtok(NULL, " ");
	}

	//debug_print_list(list);

	free(process);
	return list;
}

int get_process_id(char* token) {

	if(!strcmp(token, "processcount")) {
		return 1;
	}

	if(!strcmp(token, "runfor")) {
		return 2;
	}

	if(!strcmp(token, "use")) {
		return 3;
	}

	if(!strcmp(token, "process")) {// process name
		return 4;
	} 

	return 0;
} 

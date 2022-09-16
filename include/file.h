#ifndef PARSE_INCLUDED
#define PARSE_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processes.h"

char* get_process(char* optional_in);
instructions_* parse_process(char* process);
int get_process_id(char* token);

#endif

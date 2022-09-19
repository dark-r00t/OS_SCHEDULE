#ifndef PARSE_INCLUDED
#define PARSE_INCLUDED

#include "processes.h"

char*           get_process     (char* optional_in);
int             get_process_id  (char* token);
instructions_*  parse_process   (char* process);

#endif

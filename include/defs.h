#ifndef DEFS_INCLUDED
#define DEFS_INCLUDED

#define FCFS 0x11
#define SJF  0x22
#define RR   0x33

#define GREEN "\033[1m\033[32m"
#define CLEAR "\033[0m"

#define LOG \
    do {\
        fprintf(stderr, "\nFile: \"%s\" \nFunction: \"%s()\" \nLine: \"%i\"", __FILE__, __func__, __LINE__);\
    } while(0)
    
#endif

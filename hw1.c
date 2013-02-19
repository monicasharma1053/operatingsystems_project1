#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

//MACOSX COMMANDS
#ifdef MACOSX
#define psCommand "ps -face | awk ' NR >1{print $2\"\t\t\"$3}' | sort -n -k 2"
#endif

//MINIX COMMANDS
#ifdef MINIX
#define psCommand "ps -afe | awk ' NR >1{print $4\"\t\t\"$5}' | sort -n +1"
#endif 

//typedef
struct node {
    int pid;
    struct node *start;
    struct node *child;
    struct node *sibling;
};

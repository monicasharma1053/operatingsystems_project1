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

int customLine(char *line, FILE *ps_pipe, int size);
int growTree(struct node *node, int pid, int ppid);
struct node *findNode(struct node *node, int pid);
void printTree(struct node *tree, int indentSize);


//print tree with nodes 
void printTree(struct node *node, int indentSize)
{

    //set indents 
    char indents[128];
    //tab per line 
    char *indentLine = "\t";
    //set memory with space for indents 
    memset(indents, 0, sizeof(indents));

    int i; 
    for (i = 0; i < indentSize; i++)
    {
        strcat(indents, indentLine);
    }

    printf("%s|->%d\n",indents, node->pid);
    //increase indent for children 
    if (node->child) printTree(node->child, indentSize+1);

    //decrease indent for siblings
    if (node->sibling) printTree(node->sibling, indentSize);
}


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

//grow the tree with current or created nodes 
int growTree(struct node *node, int pid, int ppid)
{
    struct node *currentNode;
    struct node *startNode;

    currentNode = findNode(node,ppid);

    //search for currentNode 
    if (currentNode)
    {
        //initialize starting node to current found node
        startNode = currentNode;

        //go through possible children for current node 
        if (currentNode->child)
        {
            //set to child 
            currentNode = currentNode->child;
            
            //go through siblings for child node 
            while(currentNode->sibling) currentNode = currentNode->sibling;
        
            //allocate enough bytes for the node 
            currentNode->sibling = (struct node* )malloc(sizeof(struct node));
            if (NULL == currentNode->sibling) return -1;
           
                currentNode->sibling->start = startNode;
                currentNode->sibling->child = NULL;
                currentNode->sibling->sibling = NULL;
                currentNode->sibling->pid = pid;
        }

        //otherwise create a child for the current node 
        else
        {
            //allocate enough free bytes for the new node 
            currentNode->child = (struct node* )malloc(sizeof(struct node));
            currentNode->child->start = startNode;
            currentNode->child->child = NULL;
            currentNode->child->sibling = NULL;
            currentNode->child->pid = pid;
        }

    }
    //if current node isn't found set it to a new one from PPID 
    else 
    {
        currentNode = node;
        currentNode->pid = ppid; 
        //set start, child, and sibling to null for new node 
        currentNode->start = NULL;
        currentNode->child = NULL;
        currentNode->sibling = NULL;
    }
}

//retrieve a node 
struct node *findNode(struct node *node, int pid)
{
    //set current node to node in question 
    struct node *currentNode = node;

    //node null check 
    if (NULL == currentNode) 
    {
        return (struct node*)0;
    }

    //search for current node by PID
    while (currentNode)
    {
        if (currentNode->pid == pid) 
        {
            //return found match 
            return currentNode;
        }

        //check siblings 
        {
        struct node *sibling_placeholder = findNode(currentNode->sibling, pid);
        if (sibling_placeholder) 
            return sibling_placeholder;//return sibling 
        }
        //move back to the child and search again for more node matches 
        currentNode = currentNode->child;
    }
    //otherwise node is not in the tree 
    return currentNode;
}

//read the line in question 
int customLine(char *line, FILE *ps_pipe, int size)
{
    int placeholder;
    char value;
    //set space in memory for line of characters 
    memset(line, 0, size);

    for (placeholder = 0; placeholder < size-1; placeholder++)
    {
        //read and return line 
        if (1 != fread(&value, 1, 1, ps_pipe))
        {
            if (strlen(line)) return strlen(line);
            return -1;
        }
        //set line to current value 
        line[placeholder] = value;
        if ('\n' == value) return strlen(line);
    }
}

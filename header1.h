#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//DCL12-A   //DCL05-A
typedef int counter;
typedef int StackTop;
typedef char creds;

typedef struct node //DCL05-A
{
    int vertex;
    struct node *next;
} node;

typedef struct node* NODE;

typedef struct Graph //DCL05-A
{
    //DCL32-C   //DCL02-A
    int numVertices;
    bool *visited;
    int *indegree;
    node **adjLists;
} Graph;

typedef struct Graph* GRAPH;

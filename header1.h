#include <stdio.h>
#include<errno.h>
#include<string.h>
#include<math.h>


typedef struct node //DCL05-A
{
    int vertex;
    struct node *next;
} node;

typedef struct Graph //DCL05-A
{
    //DCL32-C   //DCL02-A
    int numVertices;
    bool *visited;
    int *indegree;
    node **adjLists;
} Graph;





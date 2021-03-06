#include <stdio.h>
#include<errno.h>
#include<string.h>
#include<math.h>

//DCL07-A
void push(int [], int);
int pop(int []);

//DCL12-A
void createSubjectOrder();
extern GRAPH createGraph(int);
void addEdge(Graph *, int, int);
void printAllTopologicalOrders(Graph *);

extern SIGNIN welcome(SIGNIN input);
int BOFcheck(char *, int, int);
void red();
void green();
void yellow();
void blue();
void magenta();
void cyan();
void reset();

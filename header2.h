#include <stdio.h>
#include<errno.h>
#include<string.h>
#include<math.h>

//DCL07-A
void push(int [], int);
int pop(int []);
Graph* createGraph(int);
void addEdge(Graph *, int, int);
void printAllTopologicalOrders(Graph *);
int chkAccount(char *, char *);
int login();
int registerUser();

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

void welcomeMessage();
int chkAccount(char *, char *);
int login();
int registerUser();
int studentInfo();

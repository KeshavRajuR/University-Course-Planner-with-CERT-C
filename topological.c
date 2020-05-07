#include <stdio.h>
#include <stdlib.h>
#include "header1.h"
#include "header2.h"

//INT00-A
StackTop top = -1;
counter count = 0;

//ARR31-C
void push(int arr[], int ele)
{
    arr[++top] = ele;
}

int pop(int arr[])
{
    if (top == -1)
    {
        return -1;
    }
    else
    {
        return arr[top--];
    }
}

void show(int arr[])
{
    FILE *outputFile;
    //TMP30-C
    outputFile = fopen("subjects.txt", "a");

    if (top == -1)
    {
        printf("The stack is empty.\n");
    }
    for (int i = 0; i <= top; i++)
    {
        fprintf(outputFile, "%d ", arr[i] + 1);
        if ((i % 4) == 0)
        {
            printf("\n");
        }
        printf("%d ", arr[i] + 1);
    }
    printf("\n\n");
    fprintf(outputFile, "%s", "\n");
    fclose(outputFile); //FIO46-C
}

void findAllTopologicalOrders(Graph *graph, int *stack)
{
    //for every vertex

    for (int v = 0; v < graph->numVertices; v++)
    {
        //Proceed if indegree is 0 and not yet visited

        if (graph->indegree[v] == 0 && !graph->visited[v])
        {
            struct node *adjList = graph->adjLists[v];

            while (adjList != NULL)
            {

                int connectedVertex = adjList->vertex;
                graph->indegree[connectedVertex]--;
                adjList = adjList->next;
            }

            push(stack, v);
            graph->visited[v] = true;

            //Now recursively call this function
            findAllTopologicalOrders(graph, stack);

            //Now backtracking
            adjList = graph->adjLists[v];
            while (adjList != NULL)
            {

                graph->indegree[adjList->vertex]++;
                adjList = adjList->next;
            }

            pop(stack);
            graph->visited[v] = false;
        }
    }
    if (top == (graph->numVertices - 1))
    {
        show(stack);
    }
}

void printAllTopologicalOrders(Graph *graph)
{
    int *stack;
    stack = calloc(graph->numVertices, sizeof(int));
    findAllTopologicalOrders(graph, stack);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "header1.h"
#include "header2.h"


node *createNode(int v)
{
    //MEM02-A //MEM03-A
    struct node *newNode = malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

Graph *createGraph(int vertices)
{
    //MEM02-A //MEM03-A //MEM04-A
    struct Graph *graph = malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = malloc(vertices * sizeof(node *));
    graph->visited = malloc(vertices * sizeof(bool));
    graph->indegree = malloc(vertices * sizeof(int));
    for (int i = 0; i < vertices; i++)
    {
        graph->adjLists[i] = NULL;
        graph->visited[i] = false;
        graph->indegree[i] = 0;
    }
    return graph;
}


void addEdge(struct Graph *graph, int src, int dest)
{
    // Add edge from dest to src
    struct node *newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
    graph->indegree[src]++;
}


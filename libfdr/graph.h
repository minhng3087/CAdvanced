#ifndef	_GRAPH_H_
#define	_GRAPH_H_

#include "jrb.h"
#include "dllist.h"
#define INFINITY 10000000
typedef struct Graph
{
    JRB edges;
    JRB vertices;
} Graph;
Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2, double weight);
void addEdge_notD(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
int DAG(Graph graph);
void BFS(Graph graph,int start,int stop,void (*func)(int));
void DFS(Graph graph,int start,int stop,void (*func)(int));
double shortestPath(Graph graph, int start, int stop, int *path, int *length);
void dropGraph(Graph graph);

#endif
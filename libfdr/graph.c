#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"



Graph createGraph()
{
    Graph graph;
    graph.edges = make_jrb();
    graph.vertices = make_jrb();
    return graph;
}

void addVertex(Graph graph, int id, char *name)  // add dinh 
{ 
    JRB node;
    node = jrb_find_int(graph.vertices, id);
    if(node == NULL)
        jrb_insert_int(graph.vertices, id, new_jval_s(name));
}

char *getVertex(Graph graph, int id)
{
    JRB node;
    node = jrb_find_int(graph.vertices, id);
    if(node != NULL)
        return jval_s(node->val);
    else return NULL;
}

int hasEdge(Graph graph, int v1, int v2)  // xem co duong di giua v1 va v2 khong
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if(node == NULL)
        return 0;
    tree = (JRB) jval_v(node->val);
    if(jrb_find_int(tree, v2) == NULL)
        return 0;
    else
        return 1;
}

void addEdge_notD(Graph graph, int v1, int v2, double weight)  // them dinh vao do thi khong huong
{
    JRB node, tree;
    if(!hasEdge(graph, v1, v2))
    {
        node = jrb_find_int(graph.edges, v1);
        if(node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
            tree = (JRB)jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
    if(!hasEdge(graph, v2, v1))
    {
        node = jrb_find_int(graph.edges, v2);
        if(node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v2, new_jval_v(tree));
        }
        else
            tree = (JRB)jval_v(node->val);
        jrb_insert_int(tree, v1, new_jval_d(weight));
    }
}

void addEdge(Graph graph, int v1, int v2, double weight) // do thi co huong
{
    JRB node, tree;
    if(!hasEdge(graph, v1, v2))
    {
        node = jrb_find_int(graph.edges, v1);
        if(node == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        }
        else
            tree = (JRB)jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }

    //phan bo sung
    else
    {
        tree = (JRB) jval_v(jrb_find_int(graph.edges, v1)->val);
        node = jrb_find_int(tree, v2);
        node->val = new_jval_d( jval_d(node->val) + 1 );
    }
}

double getEdgeValue(Graph graph, int v1, int v2)
{
	if(!hasEdge(graph, v1 ,v2))
        return -1;
    JRB node, tree, destination;
    node = jrb_find_int(graph.edges, v1);
    tree = (JRB) jval_v(node->val);
    destination = jrb_find_int(tree, v2);
    return jval_d(destination->val);
}

int outdegree(Graph graph, int v, int *output)  // cac dinh di ra tu dinh v
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if(node == NULL) return 0;
    tree = (JRB)jval_v(node->val);
    total = 0;
    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }
    return total;
}

int indegree(Graph graph, int v, int *output)  // cac dinh di vao dinh v
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if(jrb_find_int(tree, v) != NULL)
        {
            output[total] = jval_i(node->key);
            total ++;
        }
    }
    return total;
}

int DAG(Graph graph){
	int visited[1000];
	int n, output[100], u, v, start;
	Dllist node, stack;
	JRB vertex;
	jrb_traverse(vertex, graph.vertices){
		memset(visited, 0, sizeof(visited));

		start = jval_i(vertex->key);
		stack = new_dllist();
		dll_append(stack, new_jval_i(start));
		while(!dll_empty(stack)){
			node = dll_last(stack);
			u = jval_i(node->val);
			dll_delete_node(node);
			if(!visited[u]){
				visited[u] = 1;
				n = outdegree(graph, u, output);
				for(int i = 0; i < n; i++){
					v = output[i];
					if(v == start) return 0;
					if(!visited[v]) dll_append(stack, new_jval_i(v));
				}
			}
		}
		free_dllist(stack);
	}
	return 1;
}

void BFS(Graph graph,int start,int stop,void (*func)(int))
{
    int i,n,output[100];
    JRB node;
    JRB visited;
    visited = make_jrb();
    jrb_traverse(node, graph.edges)
        jrb_insert_int(visited, jval_i(node->key), new_jval_i(0));

    Dllist temp,queue;
    queue = new_dllist();
    dll_append(queue, new_jval_i(start));

    while(!dll_empty(queue))
    {
        temp = dll_first(queue);
        Jval value = temp->val;
        dll_delete_node(temp);
        node = jrb_find_int(visited, jval_i(value));
        if(jval_i(node->val) == 0)
        {
            func(jval_i(value));
            if(jval_i(value) == stop)
            {
                free_dllist(queue);
                return;
            }
            node->val = new_jval_i(1);
            n = outdegree(graph, jval_i(value), output);
            JRB _node;
            for(i = 0; i < n; i ++)
            {
                _node = jrb_find_int(visited, output[i]);
                if(jval_i(_node->val) == 0)
                    dll_append(queue, new_jval_i(output[i]));
            }
        }
    }
    free_dllist(queue);
}

void DFS(Graph graph,int start,int stop,void (*func)(int))
{
    JRB node;
    JRB visited;
    visited = make_jrb();
    jrb_traverse(node, graph.edges)
        jrb_insert_int(visited, jval_i(node->key), new_jval_i(0));

    Dllist stack = new_dllist();
    dll_append(stack, new_jval_i(start));

    Dllist temp;
    int n, output[100];
    int i;
    while(!dll_empty(stack))
    {
        temp = dll_last(stack);
        Jval value = temp->val;
        dll_delete_node(temp);
        node = jrb_find_int(visited, jval_i(value));
        if(jval_i(node->val) == 0)
        {
            func(jval_i(value));
            if(jval_i(value) == stop)
            {
                free_dllist(stack);
                // printf("\nTrue\n");
                return;
            }
            node->val = new_jval_i(1);
            n = outdegree(graph, jval_i(value), output);
            JRB _node;
            for(i = 0; i < n; i ++)
            {
                _node = jrb_find_int(visited, output[i]);
                if(jval_i(_node->val) == 0)
                    dll_append(stack, new_jval_i(output[i]));
            }
        }
    }
    free_dllist(stack);
    // printf("\nFalse\n");
}

double shortestPath(Graph graph, int start, int stop, int *path, int *length)  // tim duong ngan nhat Djkstra
{
    double total, distance[1000];
    int min,id,i,n,neighbor,position, previous[1000],output[1000],temp[1000];
    Dllist queue, shortest, node;

    //memset(distance, INFINITY, sizeof(distance));
    for(i = 0; i < 1000; i ++)
        distance[i] = INFINITY;

    distance[start] = 0;
    previous[start] = start;

    queue = new_dllist();
    dll_append(queue, new_jval_i(start));

    while(!dll_empty(queue))
    {
        min = INFINITY;
        dll_traverse(node, queue)
        {
            id = jval_i(node->val);
            if(min > distance[id])
            {
                min = distance[id];
                shortest = node;
            }
        }
        id = jval_i(shortest->val);
        dll_delete_node(shortest);

        if(id == stop) break;

        n = outdegree(graph, id, output);
        for(i = 0; i < n; i ++)
        {
            neighbor = output[i];
            if(distance[neighbor] > distance[id] + getEdgeValue(graph, id, neighbor))
            {
                distance[neighbor] = distance[id] + getEdgeValue(graph, id, neighbor);
                previous[neighbor] = id;
                dll_append(queue, new_jval_i(neighbor));
            }
        }
    }

    total = distance[stop];
    if(total != INFINITY)
    {
        temp[0] = stop;
        position = 1;
        while(stop != start)
        {
            stop = previous[stop];
            temp[position ++] = stop;
        }
        for(i = position - 1; i >= 0; i --)
            path[position - i - 1] = temp[i];
        *length = position;
    }
    return total;
}

void dropGraph(Graph graph)
{
    JRB node;
    jrb_traverse(node, graph.edges)
    jrb_free_tree((JRB)jval_v(node->val));
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "mapping.h"


int main(int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Input format: ./adjacent mapfile\n");
		return EXIT_FAILURE;
	}

	// Read file, build graph
	Graph * graph = readGraph(argv[1]);
	if (graph == NULL) {
		fprintf(stderr, "Unable to build graph from file\n");
		return EXIT_FAILURE;
	}


	// Build adjacency matrix or adjacency lists
	List ** adjLists = buildAdjacencyLists(graph);
	
	if (adjLists == NULL) {
		fprintf(stderr, "Unable to build adjacency matrix from graph\n");
		destroyGraph(graph);
		return EXIT_FAILURE;
	}

	printLists(adjLists, graph->vertexCount);

	destroyLists(adjLists, graph->vertexCount);
	destroyGraph(graph);
	return EXIT_SUCCESS;
}

List ** buildAdjacencyLists(Graph * g) {
	List ** lists = malloc(sizeof(*lists) * g->vertexCount);
	if (lists == NULL) {
		return NULL;
	}

	// Find all connections and order them by weight
	for (int i = 0; i < g->vertexCount; i++) {
		lists[i] = buildList(g->vertices[i], g->vertices[i]);
	}

	for (int j = 0; j < g->edgeCount; j++) {
		int ls = g->edges[j]->start->label;
		
		// Build list node, insert in correct position
		List * newList = buildList(g->edges[j]->end, g->edges[j]->start);
		List * temp = lists[ls];
		while (newList->weight < temp->weight) {
			temp = temp->next;
		}
		newList->next = temp->next;
		temp->next = newList;
	}
	return lists;
}

/*
 * Reads file + constructs graph
 * First values read are number of vertices and number of edges
 * Read in all vertices + edges and append to graph 
 */
Graph * readGraph(char * Filename) {
	FILE * fp = fopen(Filename, "r");
	if (fp == NULL) {
		return NULL;
	}

	int numVertices = 0, numEdges = 0, vertexLabel = 0, x = 0, y = 0, startInd = 0, endInd = 0;
	Graph * graph = malloc(sizeof(*graph));
	if (graph == NULL) {
		fclose(fp);
		return NULL;
	}

	fscanf(fp, "%d %d", &numVertices, &numEdges);
	graph->vertices = malloc(sizeof(*(graph->vertices)) * numVertices);
	graph->edges = malloc(sizeof(*(graph->edges)) * numEdges);
	graph->vertexCount = numVertices;
	graph->edgeCount = numEdges;

	for(int i = 0; i < numVertices; i++) {
		fscanf(fp, "%d %d %d", &vertexLabel, &x, &y);
		graph->vertices[i] = buildVertex(vertexLabel, x, y);
	}

	for (int j = 0; j < numEdges; j++) {
		fscanf(fp, "%d %d", &startInd, &endInd);
		graph->edges[j] = buildEdge(graph->vertices[startInd], graph->vertices[endInd]);
	}

	fclose(fp);
	return graph;
}

/* Build vertex */
Vertex * buildVertex(int label, int x, int y) {
	Vertex * v = malloc(sizeof(*v));
	v->label = label;
	v->x = x;
	v->y = y;
	return v;
}

/* Build edge */
Edge * buildEdge(Vertex * start, Vertex * end) {
	Edge * e = malloc(sizeof(*e));
	e->start = start;
	e->end = end;
	return e;
}

/* Build list node */
List * buildList(Vertex * ptr, Vertex * root) {
	List * list = malloc(sizeof(*list));
	list->ptr = ptr;
	list->next = NULL;
	list->weight = (int) sqrt((double) (pow(root->x - ptr->x, 2) + pow(root->y - ptr->y, 2)));
	return list;
}

void destroyLists(List ** lists, int ct) {
	for (int i = 0; i < ct; i++) {
		List * temp;
		while (lists[i] != NULL) {
			temp = lists[i];
			lists[i] = lists[i]->next;
			free(temp);
		}
	}
	free(lists);
}

/* Clears all memory associated with graph */
void destroyGraph(Graph * g) {
	for (int i = 0; i < g->vertexCount; free(g->vertices[i++]));
	free(g->vertices);
	for (int j = 0; j < g->edgeCount; free(g->edges[j++]));
	free(g->edges);
	free(g);
}

void printLists(List ** lists, int v) {
	for(int i = 0; i < v; i++) {
		List * temp = lists[i];
		while (temp != NULL) {
			printf("%d:%d -> ", temp->ptr->label, temp->weight);
			temp = temp->next;
		}
		printf("NULL\n");
	}
}

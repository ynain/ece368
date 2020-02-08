#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include "mapping.h"

#define INF INT_MAX

int main(int argc, char * argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Input format: ./proj3 mapfile queryfile\n");
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

	// Read queries, perform minimum search for each query
	if (processQueries(argv[2], adjLists, graph->vertexCount)) {
		fprintf(stderr, "Unable to process queries from file\n");
		destroyLists(adjLists, graph->vertexCount);
		destroyGraph(graph);
		return EXIT_FAILURE;
	}

	destroyLists(adjLists, graph->vertexCount);
	destroyGraph(graph);
	return EXIT_SUCCESS;
}

int processQueries(char * Filename, List ** lists, int vCount) {
	
	FILE * fp = fopen(Filename, "r");
	if (fp == NULL) {
		return 1;
	}
	
	int queryCount = 0, v1 = 0, v2 = 0;
	fscanf(fp, "%d", &queryCount);
	for (int i = 0; i < queryCount; i++) {
		fscanf(fp, "%d %d", &v1, &v2);
		// find shortest path for all nodes from source
		dijkstra(vCount, lists, 0, v1, v2);
	}
	
	fclose(fp);
	
	return 0;
}

void minimizeHeap(Heap * h, int v, int dist) {
	int a = h->pos[v];

	h->arr[a]->dist = dist;

	while(a && h->arr[a]->dist < h->arr[(a-1)/2]->dist) {
		h->pos[h->arr[a]->v] = (a-1)/2;
		h->pos[h->arr[(a-1)/2]->v] = a;
		swapNodes(&(h->arr[a]), &(h->arr[(a-1)/2]));
	
		a = (a-1)/2;
	}
	/*
	int b = (a-1)/2;
	while(a && h->arr[a]->dist < h->arr[b]->dist) {
		h->pos[h->arr[a]->v] = b;
		h->pos[h->arr[b]->v] = a;
		swapNodes(&(h->arr[a]), &(h->arr[b]));
	
		b = (b-1)/2;
	}
	*/

}

void swapNodes(HeapNode ** a, HeapNode ** b) {
	HeapNode * temp = *a;
	*a = *b;
	*b = temp;
}

HeapNode * getMinimumNode(Heap * h) {
	if (h->size == 0) {
		return NULL;
	}

	HeapNode * root = h->arr[0], * last = h->arr[h->size -1];
	h->arr[0] = last;

	h->pos[root->v] = h->size - 1;
	h->pos[last->v] = 0;

	h->size--;
	heapify(h, 0);

	return root;
}

void heapify(Heap * h, int i) {
	int min = i, l = 2*i + 1, r = 2*i + 2;

	if ((l < h->size) && (h->arr[l]->dist < h->arr[min]->dist)) {
		min = l;
	}
	if ((r < h->size) && (h->arr[r]->dist < h->arr[min]->dist)) {
		min = r;
	}

	if (min != i) {
		HeapNode * minNode = h->arr[min];
		HeapNode * iNode = h->arr[i];

		h->pos[minNode->v] = i;
		h->pos[iNode->v] = min;

		swapNodes(&(h->arr[min]), &(h->arr[i]));
		heapify(h, min);
	} 
}

void dijkstra(int a, List ** lists, int weightTotal, int src, int dest) {
	// Initialize single source
	// The main function that calulates distances of shortest paths from src to all
	//

	int * distance = malloc(sizeof(distance) * a);

	// Create minimum heap, sort by distances
	// Set distance of all nodes from source to infinity, set source dist to 0
	Heap * h = buildHeap(a, src);

	for (int i = 0; i < a; distance[i++] = INF);
	
	char ** paths = malloc(sizeof(*paths) * a);
	for (int j = 0; j < a; j++) {
		//paths[j] = malloc(sizeof(paths[0][0]) * ((j/10)+3));
		paths[j] = malloc(sizeof(paths[0][0]) * ((log10(j+1)) + 3));
		sprintf(paths[j], "%d ", j);
	}

	distance[src] = 0;
	minimizeHeap(h, src, distance[src]); 


	while (h->size != 0) {
		HeapNode * min = getMinimumNode(h);
		int m = min->v;

		destroyHeapNode(min);

		List * cur = lists[m];
		while (cur != NULL) {
			int n = cur->ptr->label;

			if ((h->pos[n] < h->size) && (distance[m] != INF) && (cur->weight+distance[m] < distance[n])) {
				distance[n] = distance[m] + cur->weight;
				minimizeHeap(h, n, distance[n]);
		
				free(paths[n]);
				char * buffer = malloc(sizeof(*buffer) * ((log10(n+1)) + 3));
				sprintf(buffer, "%d ", n);
				paths[n] = addToPath(paths[m], buffer);
				free(buffer);
			}

			cur = cur->next;
		}
	}
	
	if (distance[dest] != INF) {
		printf("%d\n", distance[dest]);
		printf("%s\n", paths[dest]);
	}
	else {
		printf("INF\n");
		printf("%d %d\n", src, dest);
	}
	destroyHeap(h);
	destroyPaths(paths, a);
	free(distance);
}

char * addToPath(char * str1, char * str2) {
	char * retval = malloc(sizeof(*retval) * (strlen(str1) + strlen(str2) + 1));
	strcpy(retval, str1);
	strcat(retval, str2);
	return retval;
}

void destroyPaths(char ** paths, int p) {
	for (int i = 0; i < p; i++){
		free(paths[i]);
	}
	free(paths);
}

Heap * buildHeap(int v, int src) {
	
	Heap * heap = malloc(sizeof(*heap));
	heap->pos = malloc(sizeof(*(heap->pos)) * v);
	heap->arr = malloc(sizeof(**(heap->arr)) * v);
	
	heap->size = v, heap->maxSize = v;
	
	for (int i = 0; i < v; i++) {
		heap->pos[i] = i;
		heap->arr[i] = buildHeapNode(i, INF);
	}
	
	heap->arr[src]->dist = 0;
	
	return heap;
}

HeapNode * buildHeapNode(int i, int dist) {
	HeapNode * node = malloc(sizeof(*node));
	node->v = i;
	node->dist = dist;
	return node;
}

int ** buildAdjacencyMatrix(Graph * g) {
	int ** matrix = malloc(sizeof(*matrix) * g->vertexCount);
	if (matrix == NULL) {
		return NULL;
	}
	for (int a = 0; a < g->vertexCount; matrix[a++] = malloc(sizeof(**matrix) * g->vertexCount));

	for (int x = 0; x < g->vertexCount; x++) {
		for (int y = 0; y < g->vertexCount; y++) {
			matrix[x][y] = 0;
		}
	}

	Vertex * st, * ed;
	for (int i = 0; i < g->edgeCount; i++) {
		st = g->edges[i]->start;
		ed = g->edges[i]->end;
		matrix[st->label][ed->label] = (int) sqrt((double) (pow(st->x - ed->x, 2) + pow(st->y - ed->y, 2)));
	}

	return matrix;
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

void destroyHeap(Heap * h) {
	if (h != NULL) {
		while (h->size > 0) {
			destroyHeapNode(h->arr[h->size - 1]);
			h->size--;
		}
		free(h->pos);
		free(h->arr);
		free(h);
	}
}

void destroyHeapNode(HeapNode * h) {
	if (h != NULL) {
		free(h);
	}
}

void destroyMatrix(int ** matrix, int ct) { 
	for (int i = 0; i < ct; free(matrix[i++]));
	free(matrix);
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

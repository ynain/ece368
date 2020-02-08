
// Single vertex in graph
typedef struct vertex {
	int label;
	int x;
	int y;
} Vertex;

// Tuple used to represent edge
typedef struct edge {
	Vertex * start;
	Vertex * end;
} Edge;

typedef struct graph {
	int vertexCount;
	int edgeCount;
	Vertex ** vertices;
	Edge ** edges;	
} Graph; 

typedef struct list {
	int weight;
	Vertex * ptr;
	struct list * next;
} List;

typedef struct heapNode {
	int v;
	int dist;
} HeapNode;

typedef struct heap {
	int size;
	int maxSize;
	int * pos;
	HeapNode ** arr;
} Heap;

/* Miscellaneous */
void printLists(List ** lists, int v);
void swapNodes(HeapNode ** a, HeapNode ** b);
void heapify(Heap * h, int i);
HeapNode * getMinimumNode(Heap * h);
char * addToPath(char * str1, char * str2);

/* Reading graph from file */
Graph * readGraph(char * Filename);

/* Build/Destroy */
Heap * buildHeap(int v, int src);
HeapNode * buildHeapNode(int i, int dist);
int ** buildAdjacencyMatrix(Graph * g);
List ** buildAdjacencyLists(Graph * g);
Vertex * buildVertex(int label, int x, int y);
Edge * buildEdge(Vertex * start, Vertex * end);
List * buildList(Vertex * ptr, Vertex * root);
void destroyPaths(char ** paths, int p);
void destroyHeap(Heap * h);
void destroyHeapNode(HeapNode * h);
void destroyMatrix(int ** matrix, int ct);
void destroyLists(List ** lists, int ct);
void destroyGraph(Graph * graph);

/* Traversal */
//int processQueries(char * filename, int ** matrix);
int processQueries(char * filename, List ** lists, int vCount);
void dijkstra(int vertexCount, List ** lists, int weightTotal, int src, int dest);

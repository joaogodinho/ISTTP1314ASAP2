#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define FALSE 0
#define TRUE 1

typedef struct Graph {
    int nNodes;
    int *nNeighbors;
    int **neighbors;
    int **nodesInSearch;
} Graph;

typedef struct {
    int *array;
    int size;
    int front;
    int rear;
    int count;
} Queue;

Graph graph;

Queue queue;
int *parent;
int *visited;
int *nCriticPoints;
int **criticPoints;


void queueInit(int size) {
    queue.size = size;
    queue.array = (int *) malloc(sizeof(int) * queue.size);
    queue.front = 0;
    queue.rear = 0;
    queue.count = 0;
}

void queueFree() { free(queue.array); }

void queueAdd(int value) {
    queue.array[queue.rear++] = value;
    queue.rear %= queue.size;
    queue.count++;
}

int queueGet() {
    int value = queue.array[queue.front++];
    queue.front %= queue.size;
    queue.count--;
    return value;
}

int queueCount() { return queue.count; }

int queueIsEmpty() {
    if (queue.count == 0) { return TRUE; }
    return FALSE;
}

void queuePrint() {
    int i;
    for (i = 0; i < queueCount(); i++) {
        printf("queue[%d] = %d\n", (queue.front + i) % queue.size, queue.array[(queue.front + i) % queue.size]);
    }
}

void queueReset() {
    queue.front = queue.rear = queue.count = 0;
}

int bfs(int source, int goal) {
    int pathFound = FALSE;
    int destination, element, vertex, i, tmp;

    for(vertex = 0; vertex < graph.nNodes; vertex++) {
        parent[vertex] = -1;
        visited[vertex] = FALSE;
    }

    queueAdd(source);
    parent[source] = -1;
    visited[source] = TRUE;

    while (!queueIsEmpty()) {
        element = queueGet();
        destination = 0;
        while (destination < graph.nNodes) {
            tmp=-1;
            for(i=0; i<graph.nNeighbors[element]; i++) {
                if(graph.neighbors[element][i] == destination) {
                    tmp = i;
                    break;
                }
            }
            if (tmp != -1 && graph.nodesInSearch[element][tmp] > 0 && !visited[destination]) {
                parent[destination] = element;
                queueAdd(destination);
                visited[destination] = TRUE;
            }
            destination++;
        }
    }

    if (visited[goal]) { pathFound = TRUE; }
    return pathFound;
}

int fordFulkerson(int source, int destination) {
    int sourceVertex, destinationVertex, u, v, i, tmp;
    int maxFlow = 0;

    tmp = 0;

    for (sourceVertex = 0; sourceVertex < graph.nNodes; sourceVertex++) {
        for (destinationVertex = 0; destinationVertex < graph.nNeighbors[sourceVertex]; destinationVertex++) {
            graph.nodesInSearch[sourceVertex][destinationVertex] = 1;
        }
    }

    while (bfs(source, destination)) {
        for (v = destination; v != source; v = parent[v]) {
            u = parent[v];

            for(i=0; i<graph.nNeighbors[u]; i++) {
                if(graph.neighbors[u][i] == v) {
                    tmp = i;
                    break;
                }
            }
            graph.nodesInSearch[u][tmp] = 0;
        }
        maxFlow++;
    }
    return maxFlow;
}

int main()
{
    int i, j, k, nNodes, nEdges, initialNode, finalNode, nCritAreas, nCritPoints, criticPoint, *results, trash = 0;

    trash = scanf(" %d %d", &nNodes, &nEdges);

    graph.nNodes = nNodes;
    graph.neighbors = (int **) malloc(sizeof(int *) * nNodes);
    graph.nNeighbors = (int *) malloc(sizeof(int) * nNodes);
    graph.nodesInSearch = (int **) malloc(sizeof(int *) * nNodes);

    queueInit(nNodes);

    parent = (int *) malloc(sizeof(int) * nNodes);
    visited = (int *) malloc(sizeof(int) * nNodes);

    for(i=0; i<nNodes; i++) {
        graph.nNeighbors[i] = 0;
        graph.neighbors[i] = NULL;
        graph.nodesInSearch[i] = NULL;
        parent[i] = -1;
        visited[i] = -1;
    }

    for (i=0; i<nEdges; i++) {
        trash = scanf(" %d %d", &initialNode, &finalNode);

        graph.neighbors[initialNode] = realloc(graph.neighbors[initialNode],
            sizeof(int) * (graph.nNeighbors[initialNode] + 1));
        graph.neighbors[initialNode][graph.nNeighbors[initialNode]] = finalNode;

        graph.neighbors[finalNode] = realloc(graph.neighbors[finalNode],
            sizeof(int) * (graph.nNeighbors[finalNode] + 1));
        graph.neighbors[finalNode][graph.nNeighbors[finalNode]] = initialNode;

        graph.nodesInSearch[initialNode] = realloc(graph.nodesInSearch[initialNode],
                                                   sizeof(int) * (graph.nNeighbors[initialNode] + 1));
        graph.nodesInSearch[initialNode][graph.nNeighbors[initialNode]] = 0;


        graph.nodesInSearch[finalNode] = realloc(graph.nodesInSearch[finalNode],
                                                   sizeof(int) * (graph.nNeighbors[finalNode] + 1));
        graph.nodesInSearch[finalNode][graph.nNeighbors[finalNode]] = 0;

        graph.nNeighbors[finalNode]++;
        graph.nNeighbors[initialNode]++;
    }

    trash = scanf(" %d", &nCritAreas);

    results = (int *) malloc(sizeof(int) * nCritAreas);
    criticPoints = (int **) malloc (sizeof(int *) * nCritAreas);
    nCriticPoints = (int *) malloc(sizeof(int) * nCritAreas);

    for(i = 0; i < nCritAreas; i++) {
        trash = scanf(" %d", &nCritPoints);
        criticPoints[i] = (int *) malloc(sizeof(int) * nCritPoints);
        nCriticPoints[i] = nCritPoints;
        for(j = 0; j < nCritPoints; j++) {
            trash = scanf( " %d", &criticPoint);
            criticPoints[i][j] = criticPoint;
        }
    }

    for (k = 0; k < nCritAreas; k++) {
        results[k] = INT_MAX;
        for (i = 0; i < nCriticPoints[k]; i++) {
            for (j = i+1; j < nCriticPoints[k]; j++) {
                trash = 0;
                queueReset();
                trash = fordFulkerson(criticPoints[k][i], criticPoints[k][j]);
                results[k] = results[k] < trash ? results[k] : trash;
            }

        }
    }


    for (i = 0; i < nCritAreas; i++) {
        printf("%d\n", results[i]);
    }

    /* Frees */
    for (i=0; i<graph.nNodes; i++) {
        free(graph.neighbors[i]);
        free(graph.nodesInSearch[i]);
    }

    free(graph.neighbors);
    free(graph.nNeighbors);
    free(graph.nodesInSearch);
    free(parent);
    free(visited);
    queueFree();
    free(results);
    for (i = 0; i < nCritAreas; i++) { free(criticPoints[i]); }
    free(criticPoints);
    free(nCriticPoints);
    return 0;
}

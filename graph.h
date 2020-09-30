
#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>

/*
 * DFS directed graph cycle search defines
 */

#define cinza -1
#define branco 0
#define preto 1

/*
 *  Graph Definition
 */

typedef enum {resource, proccess} gType;

typedef struct nodeT *nodeS;

typedef struct nodeT{
    gType nodeType;
    uintptr_t nodeCode;
    nodeS nextEdge;
    int color;
}nodeT;

typedef struct graphT *graphS;

typedef struct graphT{
    nodeS currNode;
    graphS nextNode;
}graphT;

/*
 * Graph functions headers
 */

/* Returns a Graph G with a single node */
graphS createGraph(gType nodeType, uintptr_t nodeCode);

/* Inserts a node into an already existing graph G */
int insertNode(graphS G, gType nodeType, uintptr_t nodeCode);

/* Inserts an edge into an already existing node with code K */
int insertEdge(graphS G, gType nodeType, uintptr_t nodeCode, gType newNodeType, uintptr_t newNodeCode);

/* Depth-first search in graph grafo */
int dfs(graphS grafo, nodeS vertice);

/* Checks if grasph grafo has a cycle */
int temCiclo(graphS grafo);

#endif


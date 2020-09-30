#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/*
 *  Graph Functions Definition
 */


/* Returns a Graph G with a single node */
graphS createGraph(gType nodeType, uintptr_t nodeCode){
    graphS G;

    G = malloc(sizeof(struct graphT));

    G->currNode = malloc(sizeof(struct nodeT));
    G->currNode->nextEdge = NULL;
    G->currNode->nodeCode = nodeCode;
    G->currNode->nodeType = nodeType;
    G->currNode->color = branco;

    G->nextNode = NULL;

    return G;
}

/* Inserts a node into an already existing graph G */
int insertNode(graphS G, gType nodeType, uintptr_t nodeCode){
    graphS aux;
    for(aux = G; aux->nextNode != NULL; aux = aux->nextNode){
        if(aux->currNode->nodeCode == nodeCode){
            //Node already exists
            perror("Error inserting node into graph: node already exists!");
            return -1;
        }
    }

    aux->nextNode = malloc(sizeof(struct graphT));

    aux->nextNode->currNode = malloc(sizeof(struct nodeT));
    aux->nextNode->currNode->nextEdge = NULL;
    aux->nextNode->currNode->nodeCode = nodeCode;
    aux->nextNode->currNode->nodeType = nodeType;
    aux->nextNode->currNode->color = branco;

    aux->nextNode->nextNode = NULL;

    //Successfully added node
    return 0;
}

/* Inserts an edge into an already existing node with code K */
int insertEdge(graphS G, gType nodeType, uintptr_t nodeCode, gType newNodeType, uintptr_t newNodeCode){
    graphS auxGraph;
    nodeS auxNode;

    //Finds right node for edge insertion
    for(auxGraph = G; (auxGraph->currNode->nodeType != nodeType &&
        auxGraph->currNode->nodeCode != nodeCode) ; auxGraph = auxGraph->nextNode);
    
    // auxGraph = G;
    // while((auxGraph->currNode->nodeType != nodeType) && (auxGraph->currNode->nodeCode != nodeCode)){
    //     auxGraph = auxGraph->nextNode;
    // }

    //If node with code nodeCode exists
    if(auxGraph != NULL){
        //Finds last edge
        for(auxNode = auxGraph->currNode; auxNode->nextEdge != NULL; auxNode = auxNode->nextEdge);
        
        auxNode->nextEdge = malloc(sizeof(struct nodeT));

        auxNode->nextEdge->nodeType = newNodeType;
        auxNode->nextEdge->nodeCode = newNodeCode;
        auxNode->nextEdge->color = branco;
        auxNode->nextEdge->nextEdge = NULL;

        //Successfully added new edge
        return 0;
    }else {
        //Failed to add new edge: node doesnt exist
        perror("Failed to add new edge: node doesnt exist");
        return -1;
    }
}

/* Depth-first search in graph grafo */
int dfs(graphS grafo, nodeS vertice)
{
    nodeS proxNode;
    graphS currGraph;
    int cor;
    for(proxNode = vertice->nextEdge; proxNode != NULL; proxNode = proxNode->nextEdge)
    {
        cor = proxNode->color;
        if(cor != preto)
        {
            if(cor == cinza)
                return 1;
            else
            {
                currGraph = grafo;
                while (currGraph->currNode->nodeCode != proxNode->nodeCode)
                {
                    currGraph = currGraph->nextNode;
                }
                currGraph->currNode->color = cinza;
                proxNode->color = cinza;
                if(dfs(grafo, currGraph->currNode) == 1)
                    return 1;
            }
        }
    }
    vertice->color = preto;
    return 0;
}

/* Checks if grasph grafo has a cycle */
int temCiclo(graphS grafo)
{
    graphS currGraph;

    for(currGraph = grafo; currGraph != NULL; currGraph = currGraph->nextNode)
    {
        currGraph->currNode->color = branco;
    }

    for(currGraph = grafo; currGraph != NULL; currGraph = currGraph->nextNode)
    {
        if(currGraph->currNode->color != preto)
        {
            currGraph->currNode->color = cinza;
            if(dfs(grafo, currGraph->currNode) == 1)
                return 1;
        }
    }
    return 0;
}

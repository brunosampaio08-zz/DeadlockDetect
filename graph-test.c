#include <stdio.h>
#include "graph.h"

int main(){
    graphS G;
    
    G = createGraph(proccess, 1);
    insertNode(G, proccess, 2);
    insertNode(G, proccess, 3);
    insertNode(G, proccess, 4);
    insertEdge(G, proccess, 1, proccess, 2);
    insertEdge(G, proccess, 1, proccess, 3);
    insertEdge(G, proccess, 3, proccess, 2);
    insertEdge(G, proccess, 2, proccess, 4);
    insertEdge(G, proccess, 4, proccess, 3);
    //removeEdge(G, proccess, 1, proccess, 2);
    printGraph(G);

    int k = temCiclo(G);

    if(k == 0){
        printf("Nao tem\n");
    }else {
        printf("Tem\n");
    }

    return 0;
}
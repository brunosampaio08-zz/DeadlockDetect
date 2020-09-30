#include <stdio.h>
#include "graph.h"

int main(){
    graphS G;
    
    G = createGraph(proccess, 1);
    insertNode(G, proccess, 2);
    insertNode(G, proccess, 3);
    insertEdge(G, proccess, 1, proccess, 2);
    insertEdge(G, proccess, 3, proccess, 1);

    int k = temCiclo(G);

    if(k == 0){
        printf("Nao tem\n");
    }else {
        printf("Tem\n");
    }

    return 0;
}
/* 
 *  Developed by: 
 *      Bruno Sampaio @brunosampaio08
 *      Talita Ludmila de Lima @ludmilalima
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include "graph.h"

int (*_sem_init)(sem_t *, int, unsigned int) = NULL;
int (*_sem_wait)(sem_t *) = NULL;
int (*_sem_post)(sem_t *) = NULL;
int (*_sem_getvalue)(sem_t *, int *) = NULL;

/*
 *  Global Variables Definition
 */

graphS resourcesGraph = NULL;
sem_t graphSem;

/* Inicializa o semáforo apontado em sem, indicando seu valor inicial
    especificado em value. O argumento pshared indica se o semáforo será
    compartilhados entre threads de um processos (pshared=0) ou entre
    diferentes processos (pshared<>0)*/

int sem_init(sem_t *sem, int pshared, unsigned int value){

    int r;
    uintptr_t resourceID = (uintptr_t)sem;

    if(!_sem_init){
        _sem_init = dlsym(RTLD_NEXT, "sem_init");
        _sem_wait = dlsym(RTLD_NEXT, "sem_wait");
        _sem_post = dlsym(RTLD_NEXT, "sem_post");
        _sem_getvalue = dlsym(RTLD_NEXT, "sem_getvalue");
    }

    if(resourcesGraph == NULL){
        
        //Creates a semaphore for the graph
        int k = _sem_init(&graphSem, 0, 1);
        if(k == -1){
            printf("Error creating graph semaphore! %s\n", strerror(errno));
            return k;
        }
        
        _sem_wait(&graphSem);
        //If global graph hasnt been allocated (is first init call)
        resourcesGraph = createGraph(resource, resourceID);
        _sem_post(&graphSem);

    }else{
        //Else global graph has been allocated, just add new node
        _sem_wait(&graphSem);
        insertNode(resourcesGraph, resource, resourceID);
        _sem_post(&graphSem);
    }

    r = _sem_init(sem, pshared, value);
    return r;
}

/* Decrementa (lock) o semáforo apontado em sem. Se valor do semáforo
    for maior que zero, decrementa. Se igual a zero, bloqueia quem chamou
    sem_wait(). */

int sem_wait(sem_t *sem){

    int r;
    uintptr_t resourceID = (uintptr_t)sem;
    uintptr_t proccessID = (uintptr_t)pthread_self();

    //Requests access to graph semaphore
    _sem_wait(&graphSem);
    
    //Proccess node has already been inserted?
    if(!checkNodeExists(resourcesGraph, proccess, proccessID)){
        //If false: inserts it
        insertNode(resourcesGraph, proccess, proccessID);
    }

    //If true no need to insert it: just insert request to resource edge (proccess->resource)
    insertEdge(resourcesGraph, proccess, proccessID, resource, resourceID);
    //With request added: is there a cycle?
    if(temCiclo(resourcesGraph)){
        //If true: deadlock occurred!
        printGraph(resourcesGraph);
        printf("Erro! DEADLOCK!\n");
        exit(-2);
    }else{
        //If false: Releases graph semaphore
        _sem_post(&graphSem);
        //Requests access to resource
        r = _sem_wait(sem);
        //If access has been granted: Requests access do graph semaphore
        _sem_wait(&graphSem);
        //Removes resquest edge (proccess->resource)
        removeEdge(resourcesGraph, proccess, proccessID, resource, resourceID);
        //Inserts allocation edge (resource->proccess)
        insertEdge(resourcesGraph, resource, resourceID, proccess, proccessID);
        //Releases graph semaphore
        _sem_post(&graphSem);
        return r;
    }
}

/* Incrementa (unlock) semáforo apontado em sem. Se valor do semáforo
    for zero, ao incrementar, desbloqueia outras threads/processos que
    estão bloqueados em sem_wait(). */
int sem_post(sem_t *sem){

    int r;
    uintptr_t resourceID = (uintptr_t)sem;
    uintptr_t proccessID = (uintptr_t)pthread_self();

    //Requests access to graph semaphore
    _sem_wait(&graphSem);
    //Removes resource allocation edge
    removeEdge(resourcesGraph, resource, resourceID, proccess, proccessID);
    _sem_post(&graphSem);
    //Releases graph semaphore

    //Releases resource semaphore
    r = _sem_post(sem);
    return r;
}

/* Copia o valor atual do semáforo apontado em sem para o inteiro
    apontado em sval. Chamada não é bloqueante. */
int sem_getvalue(sem_t *sem, int *sval){

    int r;

    printf("\tDentro da sem_getvaue()\n");
    r = _sem_getvalue(sem, sval);
    return(r);

}
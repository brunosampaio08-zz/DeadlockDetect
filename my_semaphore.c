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
int sleepTime = 50;

/* Inicializa o semáforo apontado em sem, indicando seu valor inicial
    especificado em value. O argumento pshared indica se o semáforo será
    compartilhados entre threads de um processos (pshared=0) ou entre
    diferentes processos (pshared<>0)*/

int sem_init(sem_t *sem, int pshared, unsigned int value){

    int r;

    if(!_sem_init){
        _sem_init = dlsym(RTLD_NEXT, "sem_init");
    }

    if(resourcesGraph == NULL){
        //If global graph hasnt been allocated (is first init call)
        resourcesGraph = createGraph(resource, (uintptr_t)sem);
        //Creates a semaphore for the graph
        int k = _sem_init(&graphSem, 0, 1);
        if(k == -1){
            printf("Error creating graph semaphore! %s\n", strerror(errno));
            return k;
        }
        //seeds the rand() function tha prevents deadlocks
        srand(time(NULL));
    }else{
        //Else global graph has been allocated, just add new node
        _sem_wait(&graphSem);
        insertNode(resourcesGraph, resource, (uintptr_t)sem);
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
    if(!_sem_wait){
        _sem_wait = dlsym(RTLD_NEXT, "sem_wait");
    }

    printf("\tDentro da sem_wait()\n");

    //_sem_wait(&graphSem);
    //If proccess doesnt exist on the graph
    if(!checkNodeExists(resourcesGraph, proccess, (uintptr_t)pthread_self())){
        //Inserts the proccess
        insertNode(resourcesGraph, proccess, (uintptr_t)pthread_self());
    }
    
    insertEdge(resourcesGraph, proccess, (uintptr_t)pthread_self(), resource, (uintptr_t)sem);

    if(temCiclo(resourcesGraph)){
        // printf("Deadlock!");
        // graphS aux;
        // //Free all of current threads semaphores
        // for(aux = findNextEdge(resourcesGraph, proccess, (uintptr_t)pthread_self());
        //     aux != NULL; aux = findNextEdge(resourcesGraph, proccess, (uintptr_t)pthread_self())){
        //         _sem_post((sem_t *)aux->currNode->nodeCode);
        //         removeEdge(resourcesGraph, aux->currNode->nodeType, aux->currNode->nodeCode,
        //             proccess, (uintptr_t)pthread_self());
        // }
        // resourcesGraph = removeNode(resourcesGraph, proccess, (uintptr_t)pthread_self());
        // _sem_post(&graphSem);
        
        // pthread_exit(NULL);
    
        exit(-1);
    }else{
        //_sem_post(&graphSem);
        r = _sem_wait(sem);
        return r;
    }
    
    // while(1){
    //     _sem_wait(&graphSem);
    //     //Proccess requests resource (proccess -> resource) -> pretend to allocate
    //     insertEdge(resourcesGraph, proccess, (uintptr_t)pthread_self(), resource, (uintptr_t)sem);
    //     //Is about to read the graph so requests access
    //     if(temCiclo(resourcesGraph)){
    //         //If cycle exists (there is a deadlock)
    //         //Free the allocated resource
    //         removeEdge(resourcesGraph, proccess, (uintptr_t)pthread_self(), resource, (uintptr_t)sem);
    //         //Releases the graph semaphore
    //         _sem_post(&graphSem);
    //         //Sleeps for a random time
    //         sleep(rand() % sleepTime);
    //     }else{
    //         //If deadlock has been solved
    //         //Releases graph semaphore
    //         _sem_post(&graphSem);
    //         //Requests access
    //         r = _sem_wait(sem);
    //         _sem_wait(&graphSem);
    //         //Access granted so removes request (proccess->resource)
    //         removeEdge(resourcesGraph, proccess, (uintptr_t)pthread_self(), resource, (uintptr_t)sem);
    //         //Adds resource alloc (resource->proccess)
    //         insertEdge(resourcesGraph, resource, (uintptr_t)sem, proccess, (uintptr_t)pthread_self());
    //         _sem_post(&graphSem);
    //         break;
    //     }
    // }
}

/* Incrementa (unlock) semáforo apontado em sem. Se valor do semáforo
    for zero, ao incrementar, desbloqueia outras threads/processos que
    estão bloqueados em sem_wait(). */
int sem_post(sem_t *sem){

    int r;
    if(!_sem_post){
        _sem_post = dlsym(RTLD_NEXT, "sem_post");
    }

    removeEdge(resourcesGraph, resource, (uintptr_t)sem, proccess, (uintptr_t)pthread_self());

    printf("\tDentro da sem_post()\n");
    r = _sem_post(sem);
    return(r);
}

/* Copia o valor atual do semáforo apontado em sem para o inteiro
    apontado em sval. Chamada não é bloqueante. */
int sem_getvalue(sem_t *sem, int *sval){

    int r;
    if(!_sem_getvalue){
        _sem_getvalue = dlsym(RTLD_NEXT, "sem_getvalue");
    }

    printf("\tDentro da sem_getvaue()\n");
    r = _sem_getvalue(sem, sval);
    return(r);

}
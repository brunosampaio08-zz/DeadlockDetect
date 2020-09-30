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
#include "graph.h"

int (*_sem_init)(sem_t *, int, unsigned int) = NULL;
int (*_sem_wait)(sem_t *) = NULL;
int (*_sem_post)(sem_t *) = NULL;
int (*_sem_getvalue)(sem_t *, int *) = NULL;

/*
 *  Global Variables Definition
 */

graphS resourcesGraph = NULL;



int sem_init(sem_t *sem, int pshared, unsigned int value){
    /* Inicializa o semáforo apontado em sem, indicando seu valor inicial
    especificado em value. O argumento pshared indica se o semáforo será
    compartilhados entre threads de um processos (pshared=0) ou entre
    diferentes processos (pshared<>0)*/

    int r;

    if(!_sem_init){
        _sem_init = dlsym(RTLD_NEXT, "sem_init");
    }

    printf("\tDentro da sem_init()\n");

    if(resourcesGraph == NULL){
        //If global graph hasnt been allocated (is first init call)
        resourcesGraph = createGraph(resource, (uintptr_t)sem);
    }else{
        //Else global graph has been allocated, just add new node
        insertNode(resourcesGraph, resource, (uintptr_t)sem);
    }

    r = _sem_init(sem, pshared, value);
        return r;
}

int sem_wait(sem_t *sem){
    /* Decrementa (lock) o semáforo apontado em sem. Se valor do semáforo
    for maior que zero, decrementa. Se igual a zero, bloqueia quem chamou
    sem_wait(). */

    int r;
    if(!_sem_wait){
        _sem_wait = dlsym(RTLD_NEXT, "sem_wait");
    }

    printf("\tDentro da sem_wait()\n");
    r = _sem_wait(sem);
    return(r);
}

int sem_post(sem_t *sem){
    /* Incrementa (unlock) semáforo apontado em sem. Se valor do semáforo
    for zero, ao incrementar, desbloqueia outras threads/processos que
    estão bloqueados em sem_wait(). */

    int r;
    if(!_sem_post){
        _sem_post = dlsym(RTLD_NEXT, "sem_post");
    }

    printf("\tDentro da sem_post()\n");
    r = _sem_post(sem);
    return(r);
}

int sem_getvalue(sem_t *sem, int *sval){
    /* Copia o valor atual do semáforo apontado em sem para o inteiro
    apontado em sval. Chamada não é bloqueante. */

    int r;
    if(!_sem_getvalue){
        _sem_getvalue = dlsym(RTLD_NEXT, "sem_getvalue");
    }

    printf("\tDentro da sem_getvaue()\n");
    r = _sem_getvalue(sem, sval);
    return(r);

}
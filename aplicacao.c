#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main(void){
    sem_t mutex;

    sem_init(&mutex,0,1);
    sem_wait(&mutex);
    
    printf("\t Na main da aplicacao depois de sem_wait()\n");
    
    return 0;
}
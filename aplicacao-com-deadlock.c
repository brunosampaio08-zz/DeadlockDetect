#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

long int c1 = 0;
sem_t s1;

long int c2 = 0;
sem_t s2;

long int c3 = 0;
sem_t s3;

void *p1(void *args){
    while(1){
        sem_wait(&s1);
        sem_wait(&s2);
        sem_wait(&s3);
        printf("P1: acessando rec 1: %ld\n", ++c1); fflush(stdout);
        printf("P1: acessando rec 2: %ld\n", ++c2); fflush(stdout);
        printf("P1: acessando rec 3: %ld\n", ++c3); fflush(stdout);
        sem_post(&s1);
        sem_post(&s2);
        sem_post(&s3);
    }
}

void *p2(void *args){
    while(1){
        sem_wait(&s2);
        sem_wait(&s3);
        sem_wait(&s1);
        printf("P2: acessando rec 1: %ld\n", ++c1); fflush(stdout);
        printf("P2: acessando rec 2: %ld\n", ++c2); fflush(stdout);
        printf("P2: acessando rec 3: %ld\n", ++c3); fflush(stdout);
        sem_post(&s2);
        sem_post(&s3);
        sem_post(&s1);
    }
}

void *p3(void *args){
    while(1){
        sem_wait(&s3);
        sem_wait(&s1);
        sem_wait(&s2);
        printf("P3: acessando rec 1: %ld\n", ++c1); fflush(stdout);
        printf("P3: acessando rec 2: %ld\n", ++c2); fflush(stdout);
        printf("P3: acessando rec 3: %ld\n", ++c3); fflush(stdout);
        sem_post(&s3);
        sem_post(&s1);
        sem_post(&s2);
    }
}

int main(){
    pthread_t t1, t2, t3;
    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 1);
    sem_init(&s3, 0, 1);
    pthread_create(&t1, NULL, p1, NULL);
    pthread_create(&t2, NULL, p2, NULL);
    pthread_create(&t3, NULL, p3, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
}

#! /bin/sh
gcc -Wall -o aplicacao aplicacao.c -lpthread
gcc -Wall -shared -o my_semaphore.so my_semaphore.c -ldl -fPIC
LD_PRELOAD=./my_semaphore.so ./aplicacao

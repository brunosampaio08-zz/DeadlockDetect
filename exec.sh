#! /bin/sh
gcc -Wall -g -o aplicacao-com-deadlock aplicacao-com-deadlock.c -lpthread
gcc -Wall -g -shared -o my_semaphore.so my_semaphore.c graph.c -ldl -fPIC
gdb aplicacao-com-deadlock

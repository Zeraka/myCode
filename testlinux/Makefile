processTest: processTest.o sem_lib.o
	gcc processTest.o sem_lib.o -o processTest
processTest.o: processTest.c sem_lib.h sem_lib.c
	gcc -c processTest.c 
sem_lib.o: sem_lib.c sem_lib.h
	gcc -c sem_lib.c 

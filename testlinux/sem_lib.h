#include<sys/sem.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int set_semvalue(int sem_id);/*初始化信号量*/
int del_semvalue(int sem_id);
int semphore_p(int sem_id);
int semphore_v(int sem_id);
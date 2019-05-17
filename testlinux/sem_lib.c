#include "sem_lib.h"
#include<string.h>


int set_semvalue(int sem_id)
{
	union semun sem_union;

	sem_union.val = 1;/*初始化信号量*/
	if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
	{
		fprintf(stderr, "set_semvalue failed\n");

		return 0;
	}

	return 1;
}

/*删除信号量*/
int del_semvalue(int sem_id)
{
	union semun sem_union;

	if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
	{
		fprintf(stderr, "del_semvalue failed\n");

		return 0;
	}

	return 1;
}

/*P操作*/
int semaphore_p(int sem_id)
{
	struct sembuf sem_buffer;

	sem_buffer.sem_num = 0;
	sem_buffer.sem_op = -1;
	sem_buffer.sem_flg = SEM_UNDO;//系统将更新进程的semadj变量
	if(semop(sem_id, &sem_buffer, 1) == -1)
	{
		fprintf(stderr, "semaphore_p failed\n", );
		
		return 0;
	}

	return 1;

}

/*V操作*/
int semaphore_v(int sem_id)
{
	struct sembuf sem_buffer;

	sem_buffer.sem_num = 0;
	sem_buffer.sem_op = 1;
	sem_buffer.sem_flg = SEM_UNDO;

	if(semop(sem_id, &sem_buffer, 1) == -1)
	{
		fprintf(stderr, "semaphore_v failed\n");

		return 0;
	}

	return 1;
}
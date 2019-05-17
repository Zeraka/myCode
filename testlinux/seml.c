#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/sem.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};//这个得自己写一个结构体

static int sem_id = 0; //信号量id为0？

static int set_semvalue(int sem_id);
static void del_semvalue(int sem_id);
static int semaphore_p(int sem_id);
static int semaphore_v(int sem_id);

int main(int argc, char *argv[])
{
	char message = 'x';
	int i = 0;

	//创建信号量，标识符为1234,信号量数1， 
	sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);

	if(argc > 1) 
	{//为什么要有这个，主要是为了区分
		//程序第一次被调用，初始化信号量
		if(!set_semvalue(sem_id))
		{
			fprintf(stderr, "failed to initialize semaphore\n");
			exit(EXIT_FAILURE);
		}

		//设置要输出到屏幕中的信息，即参数的第一个字符，如果参数大于1个，那么就会将message替换，然后打印出来
		message = argv[1][0];
		sleep(2);
	}

	for(i = 0; i < 10; i++)
	{
		//每次循环都进入临界区
		if(!semaphore_p(sem_id))
		{
			exit(EXIT_FAILURE);
		}

		printf("%c", message);//进入临界去区之后，就不断地打印信息？
		fflush(stdout);
		//清理缓冲区，然后休眠随机时间
		fflush(stdout);
		sleep(rand() % 3);

		//离开临界区
		if(!semaphore_v(sem_id))
		{
			exit(EXIT_FAILURE);
		}
		sleep(rand() % 2);
	}

	sleep(10);
	printf("\n%d finished\n", getpid());

	if(argc > 1)
	{
		//如果程序是第一次被调用，则在退出钱删除信号量
		sleep(3);
		del_semvalue(sem_id);
	}
}

static int set_semvalue(int sem_id)
{
	//初始化信号量 ，在使用信号量前必须这样做
	union semun sem_union;

	sem_union.val = 1;//因为初始化信号量，所以第三个参数为SETVAL,
	if(semctl(sem_id, 0, SETVAL, sem_union) == -1){
		fprintf(stderr, "Failed to delete semaphore\n");

		return 0;
	}
	

	return 1;
}

static void del_semvalue(int sem_id)
{
	//删除信号量
	union semun sem_union;

	if(semctl(sem_id, 0 ,IPC_RMID, sem_union) == -1)
	{
		fprintf(stderr, "Failed to delete semphore\n");
	}
}

static int semaphore_p(int sem_id)//P操作
{
	//对信号量做减1操作，即等待P(sv)
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg =SEM_UNDO;
	if(semop(sem_id, &sem_b,1) == -1 )
	{
		fprintf(stderr, "semaphore_p failed\n");
		return 0;
	};

	return 1;
}

static int semaphore_v(int sem_id)//V操作，释放，使信号量加1
{
	//释放操作，它使信号变为可用，即发送信号V(SV)
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;//信号量加1，使资源变得可用
	sem_b.sem_flg = SEM_UNDO; //SEM_UNDO是默认值

	if(semop(sem_id, &sem_b, 1) == -1)
	{
		fprintf(stderr, "semphore_v failed\n");
		return 0;
	}
	return 1;
}

/*
执行的时候，是 ./seml 0 & ./seml

在bash中使用 &  ,表明这是后台进程
*/
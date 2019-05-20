#include<sys/sem.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int set_semvalue(int sem_id);/*初始化信号量*/
void del_semvalue(int sem_id);
int semphore_p(int sem_id);
int semphore_v(int sem_id);

/*usage*/
/*
		//pid6发送消息给pid7, 使用信号量

		int sem_id = 0;
		sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
		//初始化
		if(!set_semvalue(sem_id))
		{
			fprintf(stderr, "failed to initialize semphore\n");
			exit(EXIT_FAILURE);
		}
		do{//生产者模式
			//P操作

			if(!semaphore_p(sem_id))
				exit(EXIT_FAILURE);
			printf("This is pid6 to pid7\n");

			fprintf(msgbuf, "pid6 send msg to pid7,PID6=%d\n", getpid());
			printf("in pid6 msgbuflen=%d\n",strlen(msgbuf));
			printf("msgbuf=%s\n",msgbuf);//

			
			if(!semaphore_v(sem_id))
				exit(EXIT_FAILURE);

			break;
		}while(strlen(msgbuf) == 0);

		sleep(10);
		//删除信号量
		del_semvalue(sem_id);
		exit(1);

		
		int sem_id = 0;
		sem_id = semget((key_t)1234, 0, 0666 | IPC_CREAT);//第二个参数一定要设置为0，因为该信号量已存在

		//消费者模式
		do{
			//P操作
			if(!semaphore_p(sem_id))
				exit(EXIT_FAILURE);
			printf("msgbuf len = %d\n", strlen(msgbuf));
			if(strlen(msgbuf))
				printf("pid7 gets pid6\'s msg: %s\n", msgbuf);

			if(!semaphore_v(sem_id))
				exit(EXIT_FAILURE);

		}while(strlen(msgbuf));
		exit(1);
*/
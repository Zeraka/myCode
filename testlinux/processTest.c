#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<stdarg.h>
#include<sys/wait.h>
#include<time.h>
#include<fcntl.h>
#include<limits.h>
#include<sys/stat.h>
#include<string.h>
//#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>


#define KEY (key_t)1234
#define SIZE 1024
#define CHARBUFFER 128
int msgbufSend(char * fifo_name, char * pid_Send, char * pid_Accept);
int msgbufAccept(char * fifo_name, char * pid_Accept, char * pid_Send);

int main(void)
{
	const char *fifo_name1 = "myfifo1";
	const char *fifo_name2 = "myfifo2";
	const char *fifo_name3 = "myfifo3";
	const char *fifo_name4 = "myfifo4";
	const char *fifo_name5 = "myfifo5";
	const char *fifo_name6 = "myfifo6";
	const char *fifo_name7 = "myfifo7";

	mode_t mode = 0777;

	printf("I'm your father,PID is %d.\n",getpid());//为什么只能打印一次？
	//创建7个命名管道,并且以非阻塞方式进行通信
	mkfifo(fifo_name1, mode);
	mkfifo(fifo_name2, mode);
	mkfifo(fifo_name3, mode);
	mkfifo(fifo_name4, mode);
	mkfifo(fifo_name5, mode);
	mkfifo(fifo_name6, mode);
	//mkfifo(fifo_name7, mode);//采用了信号量做法，因此废弃

	//父进程创建共享内存
	int shmid;
	char *shmaddr;
	struct shmid_ds buf;

	shmid = shmget(KEY, CHARBUFFER, IPC_CREAT | 0666);//创建
	
	if(shmid == -1){
		fprintf(stderr, "root create shm failed\n");
		exit(EXIT_FAILURE);
	}

	pid_t pid1 = fork();
	if(pid1 == 0){//第一次肯定都不是0，但第二次，一定是0
		printf("I'm pid1.PID is %d.\n",getpid());

		//pid向pid3发送消息
		msgbufSend(fifo_name1, "pid1", "pid3");

		exit(1);//若此处没有exit（1）， 进程就会执行 pid_t pid2 = fork()语句,会出现孙进程
		printf("You should never see this.\n");
	}
	pid_t pid2 = fork();
	if(pid2 == 0){
		printf("I'm pid2.PID is %d.\n",getpid());

		/*pid2向pid3发送消息*/
		msgbufSend(fifo_name2, "pid2", "pid3");

		exit(1);
		printf("You should never see this.\n");
	}
	pid_t pid3 = fork();
	if(pid3 ==0){
		printf("I'm pid3.PID is %d.\n",getpid());

		/*pid3接收pid1的消息*/
		msgbufAccept(fifo_name1, "pid3", "pid1");

		/*pid3接收pid2的消息*/
		msgbufAccept(fifo_name2, "pid3", "pid2");

		/*pid3向pid4发送消息*/
		msgbufSend(fifo_name3, "pid3", "pid4");
		/*pid3向pid5发送消息*/
		msgbufSend(fifo_name4, "pid3", "pid5");

		exit(1);
		printf("You should never see this.\n");
	}
	pid_t pid4 = fork();
	if(pid4 == 0)
	{
		printf("I'm pid4;PID=%d\n", getpid());

		/*pid4接收pid3*/
		msgbufAccept(fifo_name3, "pid4", "pid3");

		/*pid4发送消息给pid6*/
		msgbufSend(fifo_name5, "pid4", "pid6");	

		exit(1);
	}
	pid_t pid5 = fork();
	if(pid5 == 0)
	{
		printf("I'm pid5;PID=%d\n", getpid());

		/*pid5接收pid3*/
		msgbufAccept(fifo_name4, "pid5", "pid3");

		/*pid5发送到pid6*/
		msgbufSend(fifo_name6, "pid5", "pid6");

		exit(1);
	}
	pid_t pid6 = fork();
	if(pid6 == 0)
	{
		printf("I'm pid6;PID=%d\n", getpid());

		/*pid6接收pid4*/
		msgbufAccept(fifo_name5, "pid6", "pid4");

		/*pid6接收pid5*/
		msgbufAccept(fifo_name6, "pid6", "pid5");

		//pid发送消息给pid7
		shmid = shmget(KEY, CHARBUFFER, IPC_CREAT | 0666);//创建
	
		if(shmid == -1){
			fprintf(stderr, "root create shm failed\n");
			exit(EXIT_FAILURE);
		}
		shmaddr = (char*)shmat(shmid, NULL, 0);

		if(shmaddr == (void *)-1)
		{
			printf("pid6 use shm failed\n");
		}

		strcpy(shmaddr, "pid6 send msg to msg7");

		shmdt(shmaddr);
		sleep(8);
			shmctl(shmid, IPC_RMID, NULL);

		exit(1);
	}

	pid_t pid7 = fork();
	if(pid7 == 0)
	{	
		printf("I'm pid7;PID=%d\n", getpid());
		sleep(5);

		
		/*pid7 接收 pid6的消息*/
		shmaddr = (char*)shmat(shmid,NULL,0);

		if(shmaddr == (void *)-1){
			printf("pid7 shmat failed\n");
			exit(EXIT_FAILURE);
		}

		printf("pid7 get pid6 msg : %s\n", shmaddr);

		shmdt(shmaddr);
		exit(1);
	}	

	return 0;
}
 
int msgbufSend(char * fifo_name, char * pid_Send, char * pid_Accept)
{
	int pipe_fd_w = -1;
	char buffer[CHARBUFFER] = {0};
	sprintf(buffer, "%s send to %s, %s = %d", pid_Send,pid_Accept, pid_Send, getpid());

	pipe_fd_w = open(fifo_name, O_WRONLY);
	write(pipe_fd_w, buffer, CHARBUFFER);
	printf("%s send to %s successfully: %s\n", pid_Send, pid_Accept, buffer);
	close(pipe_fd_w);

	return 0;
}
int msgbufAccept(char * fifo_name, char * pid_Accept, char * pid_Send)
{
	int pipe_fd_r = -1;
	char buffer[CHARBUFFER] = {0};

	pipe_fd_r = open(fifo_name, O_RDONLY);
	read(pipe_fd_r, buffer, CHARBUFFER);
	printf("%s gets %s\'s msgbuf successfully: %s\n", pid_Accept, pid_Send, buffer);
	close(pipe_fd_r);

	return 0;
}
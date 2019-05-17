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

#include "sem_lib.h"

#define READ 0
#define WRITE 1
#define CHARBUFFER 128
int ttprintf(const char* fmt,...);

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
	const int open_mode_1 = O_WRONLY;//向管道写入模式
	const int open_mode_2 = O_RDONLY;//向管道读取模式

	printf("I'm your father,PID is %d.\n",getpid());//为什么只能打印一次？
	//创建7个命名管道,并且以非阻塞方式进行通信
	mkfifo(fifo_name1, mode);
	mkfifo(fifo_name2, mode);
	mkfifo(fifo_name3, mode);
	mkfifo(fifo_name4, mode);
	mkfifo(fifo_name5, mode);
	mkfifo(fifo_name6, mode);
	mkfifo(fifo_name7, mode);


	pid_t pid1 = fork();
	if(pid1 == 0){//第一次肯定都不是0，但第二次，一定是0
		printf("I'm pid1.PID is %d.\n",getpid());
		int pipe_fd_w = -1;//
		char buffer[CHARBUFFER]= {0};
		sprintf(buffer, "pid1 send to pid3,pid1=%d", getpid());

		//pid向pid3发送消息
		pipe_fd_w = open(fifo_name1, open_mode_1);
		write(pipe_fd_w, buffer, CHARBUFFER);
		printf("pid1 send to pid3 success: %s!\n", buffer);
		close(pipe_fd_w);

		exit(1);//若此处没有exit（1）， 进程就会执行 pid_t pid2 = fork()语句,会出现孙进程
		printf("You should never see this.\n");
	}
	pid_t pid2 = fork();
	if(pid2 == 0){
		printf("I'm pid2.PID is %d.\n",getpid());

		/*pid2向pid3发送消息*/
		int pipe_fd_w = -1;
		char buffer[CHARBUFFER] = {0};
		sprintf(buffer, "pid2 send to pid3,pid2=%d", getpid());

		if((pipe_fd_w = open(fifo_name2,open_mode_1)) < 0)
		{
			printf("error!\n");

		}
		write(pipe_fd_w, buffer,CHARBUFFER);
		printf("pid2 send to pid3 success: %s!\n", buffer);
		close(pipe_fd_w);

		exit(1);
		printf("You should never see this.\n");
	}
	pid_t pid3 = fork();
	if(pid3 ==0){
		printf("I'm pid3.PID is %d.\n",getpid());

		int pipe_fd_r = -1;
		int pipe_fd_w = -1;
		char buffer[CHARBUFFER]= {0};

		/*pid3接收pid1的消息*/
		pipe_fd_r = open(fifo_name1, open_mode_2);
		read(pipe_fd_r, buffer,CHARBUFFER);
		printf("pid3 gets pid1\'s msg successfully: %s\n", buffer);
		close(pipe_fd_r);

		/*pid3接收pid2的消息*/
		pipe_fd_r = -1;
		memset(buffer, 0, sizeof(buffer));//数组初始化
		pipe_fd_r = open(fifo_name2, open_mode_2);
		read(pipe_fd_r,buffer,CHARBUFFER);
		printf("pid3 gets pid2\'s msg successfully: %s\n", buffer);
		close(pipe_fd_r);

		/*pid3向pid4发送消息*/
		memset(buffer, 0, sizeof(buffer));
		pipe_fd_w = open(fifo_name3, open_mode_1);
		sprintf(buffer, "pid3 send to pid4,pid3=%d", getpid());
		write(pipe_fd_w, buffer, CHARBUFFER);
		printf("pid3 send pid4 success:%s\n", buffer);
		close(pipe_fd_w);

		/*pid3向pid5发送消息*/
		pipe_fd_w = -1;
		memset(buffer, 0, sizeof(buffer));
		pipe_fd_w = open(fifo_name4, open_mode_1);
		sprintf(buffer, "pid3 send to pid5, pid3=%d", getpid());
		write(pipe_fd_w, buffer, CHARBUFFER);
		printf("pid3 send to pid5 success:%s\n", buffer);
		close(pipe_fd_w);

		exit(1);
		printf("You should never see this.\n");
	}
	pid_t pid4 = fork();
	if(pid4 == 0)
	{
		printf("I'm pid4;PID=%d\n", getpid());
		int pipe_fd_r = -1;
		int pipe_fd_w = -1;
		char buffer[CHARBUFFER] = {0};

		/*pid4接收pid3*/
		pipe_fd_r = open(fifo_name3, open_mode_2);
		read(pipe_fd_r, buffer,CHARBUFFER);
		printf("pid4 get pid3 msg successfully: %s\n", buffer);
		close(pipe_fd_r);

		/*pid4发送消息给pid6*/
		memset(buffer, 0, sizeof(buffer));
		pipe_fd_w = open(fifo_name5, open_mode_1);
		sprintf(buffer, "pid4 send to pid6,pid4=%d", getpid());
		write(pipe_fd_w, buffer, CHARBUFFER);
		printf("pid4 send pid6 success:%s\n", buffer);
		close(pipe_fd_w);

		exit(1);
	}
	pid_t pid5 = fork();
	if(pid5 == 0)
	{
		printf("I'm pid5;PID=%d\n", getpid());
		int pipe_fd_r = -1;
		int pipe_fd_w = -1;
		char buffer[CHARBUFFER] = {0};

		/*pid5接收pid3*/
		pipe_fd_r = open(fifo_name4, open_mode_2);
		read(pipe_fd_r, buffer,CHARBUFFER);
		printf("pid5 get pid3 msg successfully: %s\n", buffer);
		close(pipe_fd_r);

		/*pid5发送到pid6*/
		memset(buffer, 0, sizeof(buffer));
		pipe_fd_w = open(fifo_name6, open_mode_1);
		sprintf(buffer, "pid5 send to pid6,pid5=%d", getpid());
		write(pipe_fd_w, buffer, CHARBUFFER);
		printf("pid5 send pid6 success:%s\n", buffer);
		close(pipe_fd_w);		

		exit(1);
	}
	pid_t pid6 = fork();
	if(pid6 == 0)
	{
		printf("I'm pid6;PID=%d\n", getpid());
		int pipe_fd_r = -1;
		int pipe_fd_w = -1;
		char buffer[CHARBUFFER] = {0};

		/*pid6接收pid4*/
		pipe_fd_r = open(fifo_name5, open_mode_2);
		read(pipe_fd_r, buffer,CHARBUFFER);
		printf("pid6 get pid4 msg successfully: %s\n", buffer);
		close(pipe_fd_r);	

		/*pid6接收pid5*/
		pipe_fd_r = -1;
		memset(buffer, 0, sizeof(buffer));//数组初始化
		pipe_fd_r = open(fifo_name6, open_mode_2);
		read(pipe_fd_r,buffer,CHARBUFFER);
		printf("pid6 gets pid5\'s msg successfully: %s\n", buffer);
		close(pipe_fd_r);

		/*pid6发送消息给pid7*/
		memset(buffer, 0, sizeof(buffer));
		pipe_fd_w = open(fifo_name7, open_mode_1);
		sprintf(buffer, "pid6 send to pid7,pid6=%d", getpid());
		write(pipe_fd_w, buffer, CHARBUFFER);
		printf("pid6 send pid7 success:%s\n", buffer);
		close(pipe_fd_w);	

		exit(1);
	}
	pid_t pid7 = fork();
	if(pid7 == 0)
	{	
		/*pid7 接收 pid6的消息*/
		printf("I'm pid7;PID=%d\n", getpid());
		int pipe_fd_r = -1;
		char buffer[CHARBUFFER] = {0};

		pipe_fd_r = open(fifo_name7, open_mode_2);
		read(pipe_fd_r, buffer,CHARBUFFER);
		printf("pid7 get pid6 msg successfully: %s\n", buffer);
		close(pipe_fd_r);


		exit(1);
	}

	else if(pid1 != -1){//只要是没失败，
		ttprintf("Parent forked child process--%d.\n",pid1);
		ttprintf("Parent is waiting for child to exit.\n");
		waitpid(pid1,NULL,0);
		waitpid(pid2,NULL,0);
		waitpid(pid3,NULL,0);
		waitpid(pid4,NULL,0);
		waitpid(pid5,NULL,0);
		waitpid(pid6,NULL,0);
		waitpid(pid7,NULL,0);
		ttprintf("Child Process had exited.\n");
		ttprintf("Parent had exited.\n");
	}
	else	ttprintf("Everything was done without error.\n");
 
	return 0;
}
 
 

int ttprintf(const char* fmt,...)
{
	va_list args;
	struct tm *tstruct;
	time_t tsec;
	tsec = time(NULL);
	tstruct = localtime(&tsec);
	printf("%02d:%02d:%02d:%5d|",tstruct->tm_hour,tstruct->tm_min,tstruct->tm_sec,getpid());
	va_start(args,fmt);
	return vprintf(fmt,args);
}

/*
\todo
程序可以继续优化，因为有很多重复的代码
*/
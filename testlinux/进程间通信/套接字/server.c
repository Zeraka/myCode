/*
 * 服务器端；
 * 实现从客户端获取数据并重新写回服务器；
 * 每一个客户端访问都通过子进程处理；
 * 提供并发服务；
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>

void str_echo(int sockfd);    //回射函数声明

int main(int argc, char** argv)
{
	int listenfd, connfd;        
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0); //获取监听套接字描述符
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(7777); //服务器套接字赋值
	bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)); //绑定套接字
	listen(listenfd, 5); //监听，5用来规定最大连接数

	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr*) &cliaddr, &clilen); //接受连接
		if((childpid = fork()) == 0 ) 
		{//创建子进程，由子进程处理连接
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}
}

void str_echo(int sockfd)
{
	ssize_t n;
	char buf[256] = "";  //空字符串
	while( (n = read(sockfd, buf, 256)) > 0)
	{
		write(sockfd, buf, n);
		printf("receive message: %s\n",buf);
		if(n < 0 )
			printf("read error!");
		bzero(buf, 256);    //字符串置0
	}
}
/*
 * 说明：
 * 这里buf在定义时赋值为一个空字符串，是为了在下面的printf输出中不产生乱码，
 * 因为read只读取获取的字节个数，当不足buf的256个时，buf后面为空，printf
 * 输出buf会把buf后面非接收字节输出(乱码)；
 * 没有处理子进程中止信号，子进程进入僵死状态；
 */ 

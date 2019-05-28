/*
 * 客户端；
 * 首先建立和服务器的连接，然后开始信息的传输；
 * 从标准输入端获取传输消息，发送到服务器；
 * 从服务器接收信息，输入到屏幕；
 */ 

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char** argv)
{
	int sockfd;
	struct sockaddr_in servaddr;      //服务器套接字
	char* ips = "127.0.0.1";    //服务器IP

	sockfd = socket(AF_INET, SOCK_STREAM, 0);   //获取客户端套接字描述符
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7777);  //端口号
	
	if(connect(sockfd, (struct sockaddr*)& servaddr, sizeof(servaddr)) < 0)
	{    //连接到服务器
		printf("connect error!");
		exit(0);
	}
	char sendline[256], recvline[256] = "";
	printf("Input message to be send:\n");
	while(fgets(sendline, 256, stdin) != NULL)
	{    //从标准输入获取发送信息
		if(write(sockfd, sendline, strlen(sendline)) < 0)
		{//发送消息到服务器
			printf("write message error!");
			exit(0);
		}
		if(read(sockfd, recvline, 256) < 0)
		{//接收服务器返回的消息
			printf("read message error");
			exit(0);
		}
		bzero(&recvline, 256);
		printf("Input message to be send:\n");
	}
	close(sockfd);  
	exit(0);
}
/*
 * 说明：
 * 客户端套接字没有bind其地址，由内核自动添加；
 * 当服务器关闭，客户端不会第一时间获知，再次发送信息才会读取到服务器发送的FIN；
 * 可以不断进行信息传输；
 */

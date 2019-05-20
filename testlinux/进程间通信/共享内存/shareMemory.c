#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>

#define KEY 1234    /*键，每块共享内存都有一个键值*/
#define SIZE 128   /*将要建立的共享内存的大小，1024字节*/

int main(){
  int shmid;
  char *shmaddr;
  struct shmid_ds buf;

  shmid=shmget(KEY,SIZE,IPC_CREAT|0600); /*建立共享内存*/
  if(shmid==-1){ /*建立共享内存失败则输出出错信息*/
     printf("create share memory failed:%s",strerror(errno));
     return 0;
   }

 if(fork()==0){    /*子进程*/
  shmaddr=(char*)shmat(shmid,NULL,0); /*系统自动选择一个地址让子进程链接共享内存*/

  if(shmaddr==(void*)-1){ /*链接共享内存失败则输出出错信息*/
    printf("connet to share memory failed:%s",strerror(errno));
  }
  
  strcpy(shmaddr,"hello,this is child process!\n");/*使用内存写操作strcpy往共享内存写入字符*/

  shmdt(shmaddr); /*断开与共享内存的链接*/
  return 0;

 }else{  /*父进程*/

   sleep(3); /*睡眠3秒，等待子进程结束*/

   shmctl(shmid,IPC_STAT,&buf); /*取得共享内存的相关信息，复制到缓冲区buf 中*/

   printf("size of the share memory:");
   printf("shm_segsz=%d bytes\n",buf.shm_segsz); /*打印共享内存大小*/
   printf("Process id of the creater:");
   printf("shm_cpid=%d \n",buf.shm_cpid); /*打印共享内存的创建进程id*/


   shmaddr=(char*)shmat(shmid,NULL,0); /*系统自动选择一个地址让父进程链接共享内存*/

   if(shmaddr==(void*)-1){ /*链接共享内存失败则输出出错信息*/
    printf("connet to share memory failed:%s",strerror(errno));
        return 0;
   }

   printf("the content of the share memory:"); 
   printf("%s\n",shmaddr); /*打印共享内存的内容*/

   shmdt(shmaddr); /*断开与共享内存的链接*/

   shmctl(shmid,IPC_RMID,NULL); /*当不再有任何进程使用该共享内存时系统将自动销毁它*/

 }

}

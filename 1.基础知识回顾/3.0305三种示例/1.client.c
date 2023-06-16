#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
// 作者：码农论坛 https://www.bilibili.com/read/cv7577980 出处：bilibili

#define MAX_N 1024
#define TIMES 1000000

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("usage : %s ip port\n", argv[0]);
        return -1;
    }

    // 创建网络套接字
    int sockfd;
    struct sockaddr_in servaddr;
    char buff[MAX_N];

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("socket failed.\n");
        return -1;
    }

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0){
        printf("connect(%s:%s) failed.\n", argv[1],argv[2]);
        close(sockfd);
        return -1;
    }

    printf("connect OK!\n");

    for(int ii = 0; ii < TIMES; ii++){
        memset(buff, 0, sizeof(buff));
        printf("please input : ");
        scanf("%s", buff);
        //sprintf(buff,"111111111111111 ii = %08d",ii);

        if(write(sockfd, buff, strlen(buff)) <= 0){
            printf("write() failed.\n");
            close(sockfd);
            return -1; 
        }

        memset(buff, 0, sizeof(buff));
        if (read(sockfd,buff,sizeof(buff)) <=0){
            printf("read() failed.\n"); 
            close(sockfd);
            return -1;
        }

        printf("recv:%s\n",buff);
    }

    
    return 0;
}

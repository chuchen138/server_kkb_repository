#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <ctype.h>
#include <sys/epoll.h>
// ulimit -n
#define MAXNFDS 1024
#define MAXEVENTS 100
int setnonblocking(int sockfd);
// 初始化服务端的监听端口
int initserver(int port);

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("usage: ./tcpselect port\n"); return -1;
    }
    // 初始化服务端用于监听的socket。
    int listensock = initserver(atoi(argv[1]));
    printf("listensock=%d\n",listensock);
    if (listensock < 0){
        printf("initserver() failed.\n"); return -1;
    }

    int epollfd;
    char buffer[MAXNFDS] = {0};

    // 创建描述符
    epollfd = epoll_create(1);
    struct epoll_event ev;
    ev.data.fd = listensock;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listensock, &ev);
    printf("epoll is create successfully\n");

    while(1){
        struct epoll_event events[MAXEVENTS];
        int infds = epoll_wait(epollfd,events,MAXEVENTS,-1);
        printf("epoll wait infds = %d\n", infds);
        // 返回失败
        if(infds < 0){
            printf("epoll_wait() failed.\n");
            perror("epoll_wait");
            return -1;
        }
        // 超时
        if(infds == 0){
            printf("epoll_wait() timeout.\n");
            continue;
        }
        // 检查有事情发生的socket，包括监听和客户端连接的socket
        // 检查客户端的socket事件，每次遍历集合
        for(int eventfd = 0; eventfd < infds; eventfd++){
            if(events[eventfd].data.fd == listensock && events[eventfd].events &EPOLLIN){
                // 有新的连接, 注册到数组中去
                struct sockaddr_in client;
                socklen_t client_len = sizeof(client);
                int clientfd = accept(listensock,(struct sockaddr*) &client, &client_len);
                if(clientfd < 0){
                    printf("accept() failed.\n");
                    continue;
                }
                printf("client(socket = %d) connected OK!\n", clientfd);
                if(clientfd > MAXEVENTS){
                    printf("clientsock(%d)>MAXEVENTS(%d)\n",clientfd,MAXEVENTS); close(clientfd); continue; 
                }
                memset(&ev, 0, sizeof(struct epoll_event));
                ev.data.fd = clientfd;
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD,clientfd,&ev);
                continue;
            }else if(events[eventfd].events & EPOLLIN){
                // 客户端有数据过来或客户端socket连接被断开

                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));

                // 读取客户端的数据
                ssize_t isize = read(events[eventfd].data.fd, buffer, sizeof(buffer));
                // 发生了错误或socket被对方关闭
                if(isize <= 0){
                    printf("client(eventfd = %d) disconnected.\n", events[eventfd].data.fd);
                    memset(&ev, 0, sizeof(struct epoll_event));
                    ev.data.fd = events[eventfd].data.fd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, events[eventfd].data.fd, &ev);

                    close(events[eventfd].data.fd); // 关闭客户端的socket。
                }else{
                    printf("recv(eventfd = %d, size = %ld):%s\n", events[eventfd].data.fd, isize, buffer);
                    for(int j = 0; j < strlen(buffer); j++)
                        buffer[j] = (char)toupper(buffer[j]);
                    write(events[eventfd].data.fd,buffer,strlen(buffer));
                }
            }
        }
    }
    close(epollfd);
    return 0;
}


// 初始化服务端的监听端口。

int initserver(int port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);

    if (sock < 0){
        printf("socket() failed.\n"); return -1;
    }

    // Linux如下
    int opt = 1; unsigned int len = sizeof(opt);

    // 允许套接口与一个已在使用中的地址捆绑
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,len);
    // 发送“保持活动”包
    setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,&opt,len);

    struct sockaddr_in servaddr;

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(sock,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
    {
        printf("bind() failed.\n"); close(sock); return -1;
    }
    if (listen(sock,5) != 0 )
    {
        printf("listen() failed.\n"); close(sock); return -1;
    }
    return sock;
} 



int setnonblocking(int sockfd){
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
        return -1;
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <ctype.h>
// ulimit -n
#define MAXNFDS 1024

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
    int maxfd; // fds数组中需要监视的socket的大小
    struct pollfd fds[MAXNFDS]; // fds存放在需要监视的socket
    for(int i = 0; i < MAXNFDS; i++) fds[i].fd = -1; // 初始化数组，把全部的fd设置为-1
    // 把listensock 添加到数组
    fds[listensock].fd = listensock;
    fds[listensock].events = POLLIN; 

    maxfd = listensock;

    while(1){
        int infds = poll(fds, maxfd+1, 5000);
        // printf("poll infds = %d\n", infds);
        // 返回失败
        if(infds < 0){
            printf("poll() failed.\n");
            perror("poll");
            return -1;
        }
        // 超时
        if(infds == 0){
            printf("poll() timeout.\n");
            continue;
        }
        // 检查有事情发生的socket，包括监听和客户端连接的socket
        // 检查客户端的socket事件，每次遍历集合
        for(int eventfd = 0; eventfd <= maxfd; eventfd++){
            if(fds[eventfd].fd < 0) continue;
            if((fds[eventfd].revents & POLLIN) == 0){
                continue;
            }
            fds[eventfd].revents = 0;
            if(eventfd == listensock){
                // 有新的连接, 注册到数组中去
                struct sockaddr_in client;
                socklen_t client_len = sizeof(client);
                int clientfd = accept(listensock,(struct sockaddr*) &client, &client_len);
                if(clientfd < 0){
                    printf("accept() failed.\n");
                    continue;
                }
                printf("client(socket = %d) connected OK!\n", clientfd);
                if(clientfd > MAXNFDS){
                    printf("clientsock(%d)>MAXNFDS(%d)\n",clientfd,MAXNFDS); close(clientfd); continue; 
                }
                fds[clientfd].fd = clientfd;
                fds[clientfd].events = POLLIN;
                fds[clientfd].revents = 0;
                if(maxfd < clientfd) maxfd = clientfd;
            }else{
                // 客户端有数据过来或客户端socket连接被断开

                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));

                // 读取客户端的数据
                ssize_t isize = read(eventfd, buffer, sizeof(buffer));
                // 发生了错误或socket被对方关闭
                if(isize <= 0){
                    printf("client(eventfd = %d) disconnected.\n", eventfd);
                    close(eventfd); // 关闭客户端的socket。
                    fds[eventfd].fd = -1;
                    // 重新计算maxfd的值，注意，
                    if(eventfd == maxfd){
                        for(int i = maxfd; i >= 0; i--){
                            if(fds[i].fd != -1){
                                maxfd = i;
                                break;
                            }
                        }
                    }

                    printf("maxfd = %d\n", maxfd);
                }else{
                    printf("recv(eventfd = %d, size = %ld):%s\n", eventfd, isize, buffer);
                    for(int j = 0; j < strlen(buffer); j++)
                        buffer[j] = (char)toupper(buffer[j]);
                    write(eventfd,buffer,strlen(buffer));
                }
            }
        }
    }
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


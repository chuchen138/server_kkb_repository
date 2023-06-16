/*************************************************************************
	> File Name: 1.server.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Tue 14 Mar 2023 09:38:44 PM CST
 ************************************************************************/

#include "./common/head.h"
#define MAX_EVENTS 100
#define MAX_USERS 100

const char *conf = "./wechat.conf";
struct wechat_user *users;

int main(int argc, char *argv[]){
    int opt, port = -1, server_listen;
    int sockfd, epollfd, epollfdboy, epollfdgirl;
    pthread_t tid0,tid1;
    while((opt = getopt(argc, argv,"p:")) != -1){
        switch(opt){
            case 'p':
                port = atoi(optarg);
            break;
            default:
                fprintf(stderr, "Usage : %s -p port.\n", argv[0]);
                exit(1);
        }
    }

    users = (struct wechat_user *)calloc(sizeof(struct wechat_user), MAX_USERS);
    if(port == -1){
        port = atoi(get_conf_value(conf,"PORT")); // atoi可能读到NULL
    }
    DBG(BLUE"<D>"NONE" : Server will listening in %d port.\n", port);

    if((server_listen = socket_create(port)) < 0){
        perror("socket_create");
        exit(1);
    }

    if((epollfd = epoll_create(1)) < 0){
        perror("epoll_create");
        exit(1);
    }
    if((epollfdboy = epoll_create(1)) < 0){
        perror("epollboy_create");
        exit(1);
    }
    if((epollfdgirl = epoll_create(1)) < 0){
        perror("epollgirl_create");
        exit(1);
    }
    pthread_create(&tid0,NULL, sub_reactor, (void *)&epollfdgirl);
    pthread_create(&tid1,NULL, sub_reactor, (void *)&epollfdboy);

    struct epoll_event ev,events[MAX_EVENTS];
    ev.data.fd = server_listen;
    ev.events = EPOLLIN;
    // 添加epoll套接字
    epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev);

    char str[32];
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    for(;;){
        // 等待epoll
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if(nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }

        for(int i = 0; i < nfds; ++i){
            int fd= events[i].data.fd;
            if(fd == server_listen){
                if((sockfd = accept(server_listen, (struct sockaddr*)&clientaddr, &clientlen)) < 0){
                    perror("accept");
                    exit(1);
                }
                ev.data.fd = sockfd;
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD,sockfd, &ev);
                DBG(GREEN"<EPOLL>"NONE" : new connect.\n");
                DBG(GREEN"receive %s : %d\n"NONE,inet_ntop(AF_INET,&clientaddr.sin_addr,str,32),ntohs(clientaddr.sin_port));
            }else{
                // 其他套接字，判断是什么操作

                struct wechat_msg msg;
                int ret = recv(fd,(void *)&msg, sizeof(msg), 0);
                if(ret <= 0){
                    if(ret == 0) DBG(RED"Disconnect\n"NONE);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd,NULL);
                    close(fd);
                    continue;
                }
                // DBG("ret = %d, sizeof(msg) = %ld.\n", ret, sizeof(msg));
                if(ret != sizeof(msg)){
                    DBG(RED"ret = %d, sizeof(msg)= %ld\n"NONE,ret,sizeof(msg));
                    DBG(RED"<MsgError>"NONE" : wechat msg size error.\n");
                    continue;
                }
                int sex = msg.sex;
                if(msg.type & WECHAT_SIGNUP){
                    // 注册,更新用户的信息 判断是否可以注册；
                    DBG(RED"<MsgSuccess>"NONE" : %s signup msg recved. signin again\n",msg.name);
                    msg.type = WECHAT_ACK;
                    send(fd, (void *)&msg, sizeof(msg), 0);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL,fd, NULL);
                    close(fd);
                }else if(msg.type & WECHAT_SIGNIN){
                    // 判断是否可以登录
                    DBG(GREEN"<MsgSuccess>"NONE" : %s signin msg recved.\n",msg.name);
                    msg.type = WECHAT_ACK;
                    send(fd, (void *)&msg, sizeof(msg), 0);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL,fd, NULL);
                    int epoll_tmp = sex ? epollfdboy : epollfdgirl;
                    users[fd].is_online = 1;
                    users[fd].sex = msg.sex;
                    strcpy(users[fd].name, msg.name);
                    add_to_reactor(fd, epoll_tmp);
                }
            }
        }
    }
    
    return 0;
}

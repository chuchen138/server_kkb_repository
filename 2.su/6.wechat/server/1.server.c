/*************************************************************************
	> File Name: 1.server.c
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Wed 22 Dec 2021 06:04:51 PM CST
 ************************************************************************/

#include "head.h"
#define MAXEVENTS 5
const char *conf = "./wechat.conf";
struct wechat_user *users;

int main(int argc, char **argv) {
    int opt, port = -1, server_listen, sockfd, epollfd, epollfd1, epollfd0;
    pthread_t tid0, tid1;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s -p port.\n", argv[0]);
                exit(1);
        }
    }

    if (port == -1) {
        port = atoi(get_conf_value(conf, "PORT"));
    }
    DBG(BLUE"<D>"NONE" : Server will listening on port %d.\n", port);
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        exit(1);
    }

    users = (struct wechat_user *)calloc(MAXUSERS, sizeof(struct wechat_user));

    if ((epollfd = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(1);
    }
    if ((epollfd1 = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(1);
    }
    if ((epollfd0 = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(1);
    }


    pthread_create(&tid1, NULL, sub_reactor, (void *)&epollfd1);
    pthread_create(&tid0, NULL, sub_reactor, (void *)&epollfd0);

    struct epoll_event events[MAXEVENTS], ev;
    ev.data.fd = server_listen;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev);

    for (;;) {
        int nfds = epoll_wait(epollfd, events, MAXEVENTS, -1);
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            if (events[i].data.fd == server_listen) {
                if ((sockfd = accept(server_listen, NULL, NULL)) < 0) {
                    perror("accept");
                    exit(1);
                }
                //注册
                ev.data.fd = sockfd;
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
            } else  {
                //其他套接字,判断是什么操作。
                struct wechat_msg msg;
                int ret = recv(fd, (void *)&msg, sizeof(msg), 0);
                if (ret <= 0) {
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                }
                if (ret != sizeof(msg)) {
                    DBG(RED"<MsgError>"NONE" : wechat msg size error.\n");
                    continue;
                }
                if (msg.type & WECHAT_SIGNUP) {
                    //注册，更新用户的信息到文件，判断是否可以注册
                    DBG(GREEN"<MsgSuccess>"NONE" : signup msg recved.\n");
                    msg.type = WECHAT_ACK;
                    send(fd, (void *)&msg, sizeof(msg), 0);
                } else if (msg.type & WECHAT_SIGNIN) {
                    //判断密码是否正确，以及用户是否已经登录
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                    DBG(GREEN"<MsgSuccess>"NONE" : signin msg recved.\n");
                    msg.type = WECHAT_ACK;
                    send(fd, (void *)&msg, sizeof(msg), 0);
                    int epoll_tmp = msg.sex ? epollfd1 : epollfd0;
                    users[fd].is_online = 1;
                    users[fd].sex = msg.sex;
                    strcpy(users[fd].name, msg.name);
                    add_to_reactor(epoll_tmp, fd);
                }
            }
        } 
    }

    return 0;
}

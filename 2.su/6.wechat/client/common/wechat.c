/*************************************************************************
	> File Name: wechat.c
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Wed 22 Dec 2021 08:30:01 PM CST
 ************************************************************************/

#include "head.h"
#define SUB_MAXEVENTS 5
extern struct wechat_user *users;
void send_to_all(struct wechat_msg *msg){
    for (int i = 0; i < MAXUSERS; i++) {
        if (users[i].is_online) {
            send(i, msg, sizeof(struct wechat_msg), 0);
        }
    }
}
void send_to_user(struct wechat_msg *msg){
    for (int i = 0; i < MAXUSERS; i++) {
        if (users[i].is_online && !strcmp(users[i].name, msg->to)) {
            send(i, msg, sizeof(struct wechat_msg), 0);
        }
    }
}
int add_to_reactor(int epollfd, int fd) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    make_nonblock(fd);
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        return -1;
    }
    struct wechat_msg msg;
    sprintf(msg.msg, "你的好友 %s 上线了,快和他打个招呼吧.", users[fd].name);
    msg.type = WECHAT_SYS;
    send_to_all(&msg);
    return 0;
}



void *sub_reactor(void *arg) {
    int epollfd = *(int *)arg;
    struct epoll_event ev, events[SUB_MAXEVENTS];
    for (;;) {
        int nfds = epoll_wait(epollfd, events, SUB_MAXEVENTS, -1);
        //DBG(BLUE"<epoll_wait>"NONE" : epoll return.\n");
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            struct wechat_msg msg;
            memset(&msg, 0, sizeof(msg));
            int ret = recv(fd, (void *)&msg, sizeof(msg), 0);
            if (ret <= 0) {
                DBG(RED"Connect Close by Peer.\n"NONE);
                epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                users[fd].is_online = 0;
                close(fd);
            } 
            if (sizeof(msg) != ret) {
                DBG(RED"<ChatMsgErr>"NONE" : size error.\n");
                users[fd].is_online = 0;
                close(fd);
                continue;
            }
            if (msg.type & WECHAT_WALL) {
                send_to_all(&msg);
            } else if (msg.type & WECHAT_MSG) {
                send_to_user(&msg);
            } else if (msg.type & WECHAT_FIN) {
                DBG(YELLOW"<Logout>"NONE" : "RED"%s logout.\n", msg.name);
                msg.type = WECHAT_SYS;
                sprintf(msg.msg, "你的好友 %s 离开了.", users[fd].name);
                close(fd);
                users[fd].is_online = 0;
                epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                send_to_all(&msg);
            } else {
                //Remote system call
            }

            DBG(YELLOW"<Recv> : %s\n", msg.msg);
        } 
    }
    
}



/*************************************************************************
	> File Name: wechat.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Wed 15 Mar 2023 08:28:14 PM CST
 ************************************************************************/

#include "wechat.h"

extern struct wechat_user *users;

void send_to_all(struct wechat_msg *msg){
    for(int i = 0; i < MAX_USERS; i++){
        if(!strcmp(users[i].name,msg->name)) {
            continue;
        }
        if(users[i].is_online){
            // DBG("users[i].name = %s\n",users[i].name);
            // DBG("msg.name = %s\n", msg->name);
            send(i, msg, sizeof(struct wechat_msg),0);
        }
    }
}
void send_to_user(struct wechat_msg *msg){
    for(int i = 0; i < MAX_USERS; i++){
        if(users[i].is_online && !strcmp(users[i].name,msg->to)){
            send(i, msg, sizeof(struct wechat_msg),0);
        }
    }
}
int add_to_reactor(int fd, int epollfd){
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN|EPOLLET;
    make_nonblock(fd);
    // 防止暂存区满，使IO阻塞
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD,fd,&ev) < 0){
        return -1;
    }
    struct wechat_msg msg;
    sprintf(msg.msg,"你的好友 %s 上线了，快和他打个招呼吧。\n",users[fd].name);
    msg.type = WECHAT_SYS;
    send_to_all(&msg);
    return 0;
}

void *sub_reactor(void *arg){
    int epollfd = *(int *)arg;
    struct epoll_event ev, events[SUB_MAXEVENTS];
    for(;;){
        int nfds = epoll_wait(epollfd, events, SUB_MAXEVENTS, -1);
        for(int i = 0; i < nfds; i++){
            int fd = events[i].data.fd;
            struct wechat_msg msg;
            int ret = recv(fd, (void *)&msg, sizeof(msg), 0);
            if(ret <= 0){
                epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                close(fd);
                if(ret == 0) DBG(RED"Disconnect \n"NONE);
                continue;
            }
            if(sizeof(msg) != ret){
                DBG(RED"ret = %d, msg.len = %ld\n",ret,sizeof(msg));
                DBG(RED"<ChatMsgErr>"NONE" : size err.\n");
                continue;
            }
            if(msg.type & WECHAT_WALL){
                // send_to_all
                send_to_all(&msg);
            }else if(msg.type & WECHAT_MSG){
                // send_to_user();
                send_to_user(&msg);
            }else if(msg.type & WECHAT_FIN){
                DBG(YELLOW"<Logout>"NONE" : "RED"%s logout.\n", msg.name);
                users[fd].is_online = 0;
                epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                close(fd);
                sprintf(msg.msg,"你的好友 %s 离开了\n",users[fd].name);
                msg.type = WECHAT_SYS;
                send_to_all(&msg);
            }
            else{
                // Remote system call

            }
            DBG(YELLOW"<Recv> : %s\n", msg.msg);
        }
    }
}



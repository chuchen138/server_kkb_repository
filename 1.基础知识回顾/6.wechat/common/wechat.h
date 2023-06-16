/*************************************************************************
	> File Name: wechat.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Wed 15 Mar 2023 06:59:51 PM CST
 ************************************************************************/

#ifndef _WECHAT_H
#define _WECHAT_H
#include "head.h"
struct wechat_user{
    char name[20];
    char passwd[20];
    int sex;
    int fd;
    int is_online;
};

#define SUB_MAXEVENTS 100
#define MAX_USERS 100
#define WECHAT_SIGNUP 0x01
#define WECHAT_SIGNIN 0x02
#define WECHAT_SIGOUT 0x04
#define WECHAT_ACK 0x08
#define WECHAT_NAK 0x10
#define WECHAT_WALL 0X20
#define WECHAT_MSG 0x40
#define WECHAT_SYS 0x80
#define WECHAT_FIN 0x04


struct wechat_msg{
    int type;
    int sex;
    char name[20];
    char passwd[20];
    char msg[512];
    char to[20];
};

void *sub_reactor(void *arg);
int add_to_reactor(int fd, int epollfd);
// void *client_work(void *arg);

#endif

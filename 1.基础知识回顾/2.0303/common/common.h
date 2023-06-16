/*************************************************************************
	> File Name: common.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Sat 04 Mar 2023 11:21:47 AM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
#include "head.h"

char conf_ans[512];

int make_nonblock(int fd);
int make_block(int fd);

int socket_create(int port);
int socket_create_udp(int port);

int socket_connect(const char *ip, int port); //客户端连接
int socket_connect_udp();




#endif

/*************************************************************************
	> File Name: common.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Thu 02 Mar 2023 03:17:02 PM CST
    常用的接口
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

int socket_create(int port);
int socket_connect(char *ip, int port);
int make_block(int fd);
int make_nonblock(int fd);

#endif

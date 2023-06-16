/*************************************************************************
	> File Name: 2.ipc-msg.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Sun 12 Mar 2023 05:28:34 PM CST
 ************************************************************************/

#ifndef _2.IPC-MSG_H
#define _2.IPC-MSG_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#define FTOK_FILE "1.msg.c"
const int ftok_pro = 'z';

typedef struct{
    long type;
    char content[256];
}msginfo;

#endif

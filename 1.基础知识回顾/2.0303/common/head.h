/*************************************************************************
	> File Name: head.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Fri 03 Mar 2023 05:59:43 PM CST
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include "stdio.h"
#include "string.h"
//toupper()
#include "ctype.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/time.h"
#include "sys/select.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "sys/epoll.h"
#include "./thread_pool.h"
#include "./color.h"
#include "./common.h"

#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...) 
#endif

#endif

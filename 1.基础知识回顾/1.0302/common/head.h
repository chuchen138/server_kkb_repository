/*************************************************************************
	> File Name: head.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Thu 02 Mar 2023 03:16:38 PM CST
    用来做头文件
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/wait.h"
#include "sys/types.h"
#include "color.h"
#include "common.h"

#include "pthread.h"
#include "thread_pool.h"

#ifdef _D
#define DBG(fmt, args...) printf(fmt,##args)
#else
#define DBG(fmt, args...)
#endif

#endif

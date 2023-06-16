/*************************************************************************
	> File Name: thread_pool.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Fri 03 Mar 2023 06:07:15 PM CST
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include "head.h"

struct task_queue{
    int head,tail,size,total;
    void **data;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
typedef struct task_queue* TQ;

void task_queue_init(TQ tq, int size);
void task_queue_push(TQ tq, void *data);
void *task_queue_pop(TQ tq);

void *run_thread_pool(void *arg);
void *thread_work(void *arg);


#endif

/*************************************************************************
	> File Name: thread_pool.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Thu 02 Mar 2023 03:34:31 PM CST
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

struct task_queue{
    int head, tail, size, total;
    void **data;
    pthread_mutex_t mutex;
    pthread_cond_t cond; // 提醒客人来了
};

typedef struct task_queue* TQ;
void task_queue_init(TQ tq, int size);
void task_queue_push(TQ tq, void *data);
void task_queue_pop(TQ tq, void *data);

void *thread_run(void *arg);

#endif

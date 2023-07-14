#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include "head.h"
struct task_queue{
// struct TQ{
    pthread_mutex_t mutex;
    pthread_cond_t cond; // 提醒客人来了
    int head, tail, size, total;
    void **data;
    int *nfds;
};
// typedef struct task_queue* TQ;
typedef task_queue* TQ;
void task_queue_init(TQ tq, int size);
void task_queue_push(TQ tq, void *data, int fd);
void *task_queue_pop(TQ tq, int *fd);

// void *run_thread_pool(void *arg);
// void *thread_work(void *arg);


#endif


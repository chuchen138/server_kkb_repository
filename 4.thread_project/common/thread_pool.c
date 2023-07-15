#include "head.h"


void task_queue_init(TQ tq, int size){
    tq->size = size;
    tq->head = tq->tail = tq->total = 0;
    tq->data = (void **)calloc(size, sizeof(void *));
    tq->nfds = (int *) malloc(size * sizeof(int));
    pthread_mutex_init(&tq->mutex, NULL);
    pthread_cond_init(&tq->cond, NULL);
}

void task_queue_push(TQ tq, void *data, int fd){
    DBG("in task_queue_push.\n");
    // 1.加锁
    pthread_mutex_lock(&tq->mutex);
    DBG("in task_queue_push.\n");
    if(tq->total == tq->size){
        DBG(RED "<Push> queue is full!\n" NONE);
        pthread_mutex_unlock(&tq->mutex);
        return;
    }
    // 2.增加总和，增加数据
    tq->total++;
    tq->nfds[tq->tail] = fd;
    DBG("task_queue_push fd = %d.\n", fd);
    tq->data[tq->tail] = data;
    DBG(YELLOW "<Push> [%d] successfully!\n" NONE, tq->tail);
    // 3.判断尾部是否超界
    if(++tq->tail == tq->size){
        tq->tail = 0;
        DBG(BLUE "<Push> tail is zero.\n" NONE);
    }
    // 4.解锁
    pthread_cond_signal(&tq->cond);
    pthread_mutex_unlock(&tq->mutex);
}


void* task_queue_pop(TQ tq, int *fd_ptr){
    // 1.加锁
    pthread_mutex_lock(&tq->mutex);
    // 2.当队列为空，循环等待
    while(tq->total == 0){
        pthread_cond_wait(&tq->cond,&tq->mutex);
        DBG(RED "<Pop> queue is empty!\n" NONE);
    }
    // 3.总和减一，头减一
    void *data = tq->data[tq->head];
    *fd_ptr = tq->nfds[tq->head];
    DBG("task_queue_pop fd = %d.\n", *fd_ptr);
    tq->total--;
    DBG(YELLOW "<Pop> [%d] successfully!\n" NONE, tq->head);
    if(++tq->head == tq->size){
        tq->head = 0;
        sleep(3);
        DBG(BLUE "<Pop> tq->head is zero!\n" NONE);
    }
    // 4.解锁
    pthread_mutex_unlock(&tq->mutex);
    return data;
    return (void *)0;
}


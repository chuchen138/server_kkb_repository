#include "head.h"

void task_queue_init(TQ tq, int size) {
    tq->size = size;
    tq->total = tq->head = tq->tail = 0;
    tq->data = calloc(size,sizeof(void*));
    pthread_mutex_init(&tq->mutex,NULL);
    pthread_cond_init(&tq->cond,NULL);
}



void task_queue_push(TQ tq, void *data){
    pthread_mutex_lock(&tq->mutex);
    if(tq->size == tq->total){
        DBG(RED"<Push fail> task_queue is full... from %lu.."NONE,pthread_self());
        pthread_mutex_unlock(&tq->mutex);
        return;
    }
//        printf("task_queue_push : %s\n",(char *)data);

    tq->data[tq->tail] = data;
    DBG(BLUE"<push> Push to %dth task. from %lu..\n"NONE, tq->tail, pthread_self());
    tq->total++;
    if(++tq->tail == tq->size){
        DBG(YELLOW"<Push> tail begins with 0. from %lu..\n"NONE, pthread_self());
        tq->tail = 0;
    }
    pthread_cond_signal(&tq->cond);
    pthread_mutex_unlock(&tq->mutex);
}


void task_queue_pop(TQ tq, void *data){
    pthread_mutex_lock(&tq->mutex);
    while(tq->total == 0){
        DBG(RED"<Pop fail> No data in task_queue. from %lu..\n"NONE, pthread_self());
        // pthread_mutex_unlock(&tq->mutex);
        pthread_cond_wait(&tq->cond, &tq->mutex);
    }
    tq->total--;
    // data = tq->data[tq->head];
    memcpy(data, tq->data[tq->head], 1024);
    // printf("task_queue_pop : %s\n",(char *)data);
    DBG(BLUE"<Pop> pop %dth task. from %lu..\n"NONE, tq->head, pthread_self());
    if(++tq->head == tq->size){
        tq->head = 0;
        DBG(YELLOW"<Pop> head begins with 0. from %lu..\n"NONE, pthread_self());
    }
    pthread_mutex_unlock(&tq->mutex);
}


void *thread_run(void *arg){
    pthread_detach(pthread_self());
    TQ tq = (TQ)arg;
    char *data = malloc(1024);
    while(1){
        memset(data,0,sizeof(data));
        task_queue_pop(tq,(void *)data);
        printf("%s\n",/*(char *)*/data);
        DBG(GREEN"<thread> got a task. from %lu..\n"NONE, pthread_self());
    }
}

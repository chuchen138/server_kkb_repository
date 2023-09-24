/*************************************************************************
	> File Name: thread_pool.c
	> Author: suyelu 
	> Mail: suyelu@126.com
	> Created Time: Fri 03 Dec 2021 06:22:02 PM CST
 ************************************************************************/

#include "head.h"
extern char *data[2000];
extern int epollfd;
extern pthread_mutex_t mutex[2000];

void task_queue_init(struct task_queue *taskQueue, int size) {
    taskQueue->size = size;
    taskQueue->total = taskQueue->head = taskQueue->tail = 0;
    taskQueue->data = calloc(size, sizeof(void *));
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
    return ;
}


void task_queue_push(struct task_queue *taskQueue, void *data) {
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->total == taskQueue->size) {
        DBG(YELLOW"<push> taskQueue is full.\n"NONE);
        pthread_mutex_unlock(&taskQueue->mutex);
        return ;
    }
    taskQueue->data[taskQueue->tail] = data;
    DBG(PINK"<push> push to %dth task.\n"NONE, taskQueue->tail);
    taskQueue->total++;
    if (++taskQueue->tail == taskQueue->size) {
        DBG(PINK"<push> tail begins with 0.\n"NONE);
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
}

void *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    while  (taskQueue->total == 0) {
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    void *data = taskQueue->data[taskQueue->head];
    DBG(BLUE"<pop> pop data from %dth task.\n"NONE, taskQueue->head);
    taskQueue->total--;
    if (++taskQueue->head == taskQueue->size) {
        DBG(BLUE"<pop> head begins with 0.\n"NONE);
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return data;
}


void *thread_run(void *arg) {
    pthread_detach(pthread_self());
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        void *data = task_queue_pop(taskQueue);
        DBG(GREEN"<thread> got a task.\n"NONE);
        printf("%s", data);
    }
}

void do_work(int fd) {
    char buff[4096] = {0};
    int rsize = -1;
    DBG(YELLOW"<Recv> : Before Recv.\n");
    int ind = strlen(data[fd]);
    DBG(YELLOW"<Recv> : Before Recv.\n");
    if ((rsize = recv(fd, buff, sizeof(buff), 0)) <= 0) {
        epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
        DBG(RED"<Close> : connection closed by peer!\n"NONE);
        close(fd);
        return ;
    }
    pthread_mutex_lock(&mutex[fd]);
    DBG(YELLOW"<Recv> : recv from client.\n");
    for (int i = 0; i < rsize; i++) {
        if (buff[i] >= 'A' && buff[i] <= 'Z') {
            data[fd][ind++] = buff[i] + 32;
        } else if (buff[i] >= 'a' && buff[i] <= 'z') {
            data[fd][ind++] = buff[i] - 32;
        } else {
            data[fd][ind++] = buff[i];
            if (buff[i] == '\n'){
                DBG(GREEN"<Success> Send data to clients!\n"NONE);
                send(fd, data[fd], ind, 0);
            }
        }
    }
    pthread_mutex_unlock(&mutex[fd]);
}
void *thread_work(void *arg) {
    pthread_detach(pthread_self());
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        int *fd = (int *)task_queue_pop(taskQueue);
        DBG(GREEN"<thread> got a task.\n"NONE);
        do_work(*fd);
    }
}

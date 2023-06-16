#include "head.h"

extern int epollfd;
extern char *data[20000];
extern pthread_mutex_t mutex[20000];

void task_queue_init(TQ tq, int size){
    tq->size = size;
    tq->head = tq->tail = tq->total = 0;
    tq->data = calloc(size, sizeof(void *));
    pthread_mutex_init(&tq->mutex, NULL);
    pthread_cond_init(&tq->cond, NULL);
}

void task_queue_push(TQ tq, void *data){
    DBG("in task_queue_push.\n");
    // 1.加锁
    pthread_mutex_lock(&tq->mutex);
    DBG("in task_queue_push.\n");
    if(tq->total == tq->size){
        DBG(RED"<Push> queue is full!\n"NONE);
        pthread_mutex_unlock(&tq->mutex);
        return;
    }
    // 2.增加总和，增加数据
    tq->total++;
    DBG("task_queue_push fd = %d.\n", *(int *)data);
    tq->data[tq->tail] = data;
    DBG(YELLOW"<Push> [%d] successfully!\n"NONE, tq->tail);
    // 3.判断尾部是否超界
    if(++tq->tail == tq->size){
        tq->tail = 0;
        DBG(BLUE"<Push> tail is zero.\n"NONE);
    }
    // 4.解锁
    pthread_cond_signal(&tq->cond);
    pthread_mutex_unlock(&tq->mutex);
}


void *task_queue_pop(TQ tq){
    // 1.加锁
    pthread_mutex_lock(&tq->mutex);
    // 2.当队列为空，循环等待
    while(tq->total == 0){
        pthread_cond_wait(&tq->cond,&tq->mutex);
        DBG(RED"<Pop> queue is empty!\n"NONE);
    }
    // 3.总和减一，头减一
    int *data = tq->data[tq->head];
    DBG("task_queue_pop fd = %d.\n", *data);
    tq->total--;
    DBG(YELLOW"<Pop> [%d] successfully!\n"NONE, tq->head);
    if(++tq->head == tq->size){
        tq->head = 0;
        sleep(3);
        DBG(BLUE"<Pop> tq->head is zero!\n"NONE);
    }
    // 4.解锁
    pthread_mutex_unlock(&tq->mutex);
    return data;
    //return (void *)0;
}

void *run_thread_pool(void *arg){
    pthread_detach(pthread_self());
    TQ tq = (TQ)arg;
    while(1){
        char *data = (char *)task_queue_pop(tq);
        DBG(GREEN"%s\n"NONE,data);
    }


}

static void do_work(int fd){
    char buff[4096] = {0};
    int rsize = -1, ind = 0;
    if((rsize = recv(fd, buff, sizeof(buff),0)) <= 0){
        DBG("recv rsize = %d\n", rsize);
        epoll_ctl(epollfd,EPOLL_CTL_DEL, fd,NULL);
        close(fd);
        DBG(RED"<Close> : connection(%d) closed by peer!\n"NONE, fd);
        return;
    }
    DBG("recv : %s.\n", buff);
    if(data[fd] == NULL) data[fd] = (char *)calloc(sizeof(char), 4096);
    pthread_mutex_lock(&mutex[fd]);
    DBG("recv : %s.\n", buff);
    for(int i = 0; i < rsize; i++){
        if(buff[i] >= 'A' && buff[i] <= 'Z'){
            data[fd][ind++] = buff[i]+32;
        }else if(buff[i] >= 'a' && buff[i] <= 'z'){
            data[fd][ind++] = buff[i]-32;
        }else{
            data[fd][ind++] = buff[i];
            if(buff[i] == '\n'){
                break;
            }

        }
    }
    send(fd, data[fd], ind, 0);
    DBG(GREEN"SEND(%d,%s)\n"NONE, fd,data[fd]);
    DBG("recv : %s.\n", buff);
    pthread_mutex_unlock(&mutex[fd]);
}

void *thread_work(void *arg){
    pthread_detach(pthread_self());
    TQ tq = (TQ)arg;
    char buff[1024];
    // char s[6] = "sxg->";
    // DBG(GREEN"thread_pid = %ld,test : %s\n"NONE,pthread_self(),s);
    while(1){
        int *fd = task_queue_pop(tq);
        DBG(GREEN"<thread> got a task, fd = %d.\n"NONE, *fd);
        do_work(*fd);
/*
        pthread_mutex_lock(&tq->mutex);
        int fd = data[0];
        int rsize = read(fd, buff,1024);
        if(rsize < 0){
            pthread_mutex_unlock(&tq->mutex);
            DBG(RED"fd = (%d),disconnected!\n"NONE, fd);
            perror("read");
            // epoll_ctl(epollfd,EPOLL_CTL_DEL, fd,NULL);
            close(fd);
            continue;
        }
        DBG(GREEN"thread_pid = %ld, recv(rsize = %d) : %s\n"NONE,pthread_self(),rsize,buff);
        for(int i = rsize; i >= 0; --i){
            buff[i+5] = toupper(buff[i]);
        }
        for(int i = 0; i < 5; ++i) buff[i] = s[i];
        if(write(fd, buff, rsize+5) < 0) DBG(RED"fd = (%d)write fail.\n"NONE,fd);
        epoll_ctl(epollfd,EPOLL_CTL_DEL, fd,NULL);
        close(fd);
        pthread_mutex_unlock(&tq->mutex);
        // close(fd);
*/
    }
    
}





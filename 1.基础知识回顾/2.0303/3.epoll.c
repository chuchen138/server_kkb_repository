/*************************************************************************
	> File Name: 3.epoll.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Mon 06 Mar 2023 07:56:58 PM CST
 ************************************************************************/

#include "./common/head.h"

#define MAXUSER 20000 // 最大用户数
#define MAXEVENTS 10
#define INS 2 // 线程池
#define QUEUESIZE 100

int clients[MAXUSER];
int epollfd;
int main(int argc, char *argv[]){
    
    if(argc < 2){
        fprintf(stderr, "Usage : %s port.\n", argv[0]);
        exit(1);
    }

    int serve_listen, port, sockfd;
    port = atoi(argv[1]);
    if((serve_listen = socket_create(port)) < 0){
        perror("socket_create");
        exit(1);
    }

    DBG(YELLOW"<Init> : serve_listen %d starts on port %d.\n", serve_listen, port);
    TQ tq = (TQ)malloc(sizeof(struct task_queue));
    task_queue_init(tq, QUEUESIZE);
    DBG(YELLOW"<Init> : task_queue_init.\n"NONE);


    pthread_t *tid = (pthread_t *)calloc(INS,sizeof(pthread_t));
    for(int i = 0; i < INS; i++){
        pthread_create(&tid[i],NULL, thread_work, (void *)tq);
    }
    DBG(YELLOW"<Init> : work threads create.\n"NONE);

    if((epollfd = epoll_create(1)) < 0){
        perror("epoll_create.");
        exit(1);
    }

    struct epoll_event ev, events[MAXEVENTS];
    ev.data.fd = serve_listen;
    ev.events = EPOLLIN;

    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, serve_listen, &ev) < 0){
        perror("epoll_create.");
        exit(1);
    }
    // make_nonblock(serve_listen);

    DBG(YELLOW"<Init> : EPOLL Instance created and add serve_listen in.\n"NONE);

    int tmp = 0;
    for(;;){
        int nfds = epoll_wait(epollfd, events, MAXEVENTS, -1);
        if(nfds < 0){
            perror("epoll_wait");
            exit(1);
        }
        printf("nfds = %d\n",nfds);

        //if(++tmp == 9) break;
        for(int i = 0; i < nfds; i++){
            int fd = events[i].data.fd;
            if(fd == serve_listen && (events[i].events & EPOLLIN)){
                // DBG(BLUE"<Epoll> new connect.\n");
                if((sockfd = accept(serve_listen, NULL, NULL)) < 0){
                    perror("accept");
                    continue;
                }
                clients[fd] = sockfd;
                ev.data.fd = sockfd;
                ev.events = EPOLLET|EPOLLIN;
                make_nonblock(sockfd);
                if(epoll_ctl(epollfd,EPOLL_CTL_ADD,sockfd, &ev) < 0){
                    perror("epoll_ctl");
                    close(sockfd);
                    continue;
                }

                DBG(BLUE"<Epoll> new connect. fd = %d\n",sockfd);
            }else /*if(events[i].events & EPOLLET)*/{
                DBG("cnt = %d\n",tmp++);
                DBG(BLUE"<Epoll> recv infomation.\n");
                task_queue_push(tq, (void *)fd);
                // epoll_ctl(epollfd, EPOLL_CTL_DEL, clients[fd],NULL);
            }
        }
    }



    //epoll 
    return 0;
}

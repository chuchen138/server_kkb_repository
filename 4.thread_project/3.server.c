#include "common/head.h"
#include "common/ret_value.h"
#include "common/mess_type.h"
#include "common/worker.h"
#include "UserManager.h"
#include "RelationManager.h"
#include "MessageManager.h"
#include "PhotoManager.h"
#include "pb/message_define.pb.h"
#include "BusManager.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/ipc.h>
#include <sys/shm.h>

UserManager user_svr;
RelationManager rela_svr;
MessageManager mess_svr;
PhotoManager photo_svr;
DbManager db_svr;
BusManager bus_svr;

#define INS 4
#define MAX 100
#define MAX_DATA 100
#define MAX_EVENTS 100
#define MAX_USERS 100

char recv_buffer[MAX_DATA+5][4100];
const char *conf = "./wechat.conf";

int main(int argc, char *argv[]){
    db_svr.Init();
    user_svr.Init(&db_svr);
	user_svr.Start();
	rela_svr.Init(&db_svr);
	rela_svr.Start();
	mess_svr.Init(&db_svr);
	mess_svr.Start();
	photo_svr.Init(&db_svr);
	photo_svr.Start();

    int port = -1, server_listen;
    int sockfd, epollfd;

    // 初始化线程池
    pthread_t tid[INS];
    TQ tq = (TQ)malloc(sizeof(struct task_queue));
    task_queue_init(tq, MAX);
    // if(port == -1){
    //     port = atoi(get_conf_value(conf,"PORT")); // atoi可能读到NULL
    // }
    port = 8999;
    DBG(BLUE "<D>" NONE " : Server will listening in %d port.\n", port);

    // 创建socket
    if((server_listen = socket_create(port)) < 0){
        perror("socket_create");
        exit(1);
    }
    // epoll初始化
    if((epollfd = epoll_create(1)) < 0){
        perror("epoll_create");
        exit(1);
    }
    // 创建线程
    for(int i = 0; i < INS; i++){
        pthread_create(&tid[i], NULL, thread_work, (void *)tq);
        printf(GREEN "%dth thread : %lu\n" NONE, i, tid[i]);
    }

    struct epoll_event ev,events[MAX_EVENTS];
    ev.data.fd = server_listen;
    ev.events = EPOLLIN;
    // 添加epoll套接字
    epoll_ctl(epollfd, EPOLL_CTL_ADD, server_listen, &ev);
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);

    char str[32];
    int mess_count = 0;
    for(;;){
        // 等待epoll
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if(nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for(int i = 0; i < nfds; ++i){
            int fd= events[i].data.fd;
            if(fd == server_listen){
                if((sockfd = accept(server_listen, (struct sockaddr*)&clientaddr, &clientlen)) < 0){
                    perror("accept");
                    exit(1);
                }
                ev.data.fd = sockfd;
                ev.events = EPOLLIN;
                epoll_ctl(epollfd, EPOLL_CTL_ADD,sockfd, &ev);
                DBG(GREEN "<EPOLL>" NONE " : new connect.\n");
                DBG(GREEN "receive %s : %d\n" NONE,inet_ntop(AF_INET,&clientaddr.sin_addr,str,32),
                            ntohs(clientaddr.sin_port));
            }else{
                // 其他套接字，判断是什么操作
                int ret = recv(fd, recv_buffer[mess_count],4096, 0);
                if(ret <= 0){
                    if(ret == 0) DBG(RED "Disconnect\n" NONE);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd,NULL);
                    close(fd);
                    continue;
                }
                DBG(GREEN "<RECV>" NONE " :main fd = %d.\n", fd);
                // DBG(GREEN "<RECV>" NONE " : recvbuff = [%s]\n", recv_buffer[mess_count]);
                task_queue_push(tq, recv_buffer[mess_count],fd);
                mess_count++;
                // DBG("ret = %d, sizeof(msg) = %ld.\n", ret, sizeof(msg));
            }
        }
        user_svr.Proc();
		rela_svr.Proc();
		mess_svr.Proc();
		photo_svr.Proc();
        printf(NONE "proc  \n\n" NONE);
        usleep(5000);/* 5ms */
    }
	user_svr.Shutdown();
	rela_svr.Shutdown();
	mess_svr.Shutdown();
    photo_svr.Shutdown();
    return 0;
}

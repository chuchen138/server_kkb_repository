/*************************************************************************
	> File Name: 2.select.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Sat 04 Mar 2023 02:37:40 PM CST
 ************************************************************************/

#include "head.h"

#define MAX 100
#define INS 5

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Usage: %s port.\n", argv[0]);
        return 0;
    }

    int server_listen, port, sockfd;
    int clients[MAX] = {0};
    port = atoi(argv[1]);

    pthread_t tid[INS];
    char buff[MAX][1024];
    TQ tq = (TQ)malloc(sizeof(struct task_queue));
    task_queue_init(tq,MAX);

    for(int i = 0; i < INS; i++){
        pthread_create(&tid[i], NULL, run_thread_pool, (void *)tq);
    }
    if((server_listen = socket_create(port)) < 0){
        perror("server_listen");
        exit(1);
    }
    // printf("连接成功\n");

    int max_fd;

    fd_set rfds;
    struct timeval tv;
    FD_ZERO(&rfds);
    FD_SET(server_listen, &rfds);
    max_fd = server_listen;
    clients[server_listen] = server_listen;

    while(1){
        FD_ZERO(&rfds);
        FD_SET(server_listen, &rfds);
        for(int i = 3; i <= max_fd ; i++){
            if(clients[i] == -1) continue;
            FD_SET(clients[i], &rfds);
        }
        int ret = select(max_fd+1, &rfds, NULL,NULL,NULL);
        if(ret < 0){
            perror("select");
            exit(1);
        }
        if(FD_ISSET(server_listen, &rfds)){
            if((sockfd = accept(server_listen, NULL,NULL)) < 0){
                perror("accept");
                return -1;
            }
            ret--;
            clients[sockfd] = sockfd;
            if(sockfd > max_fd) max_fd = sockfd;

        }
        for(int i = 0; i < max_fd+1; i++){
            if(clients[i] == server_listen) continue;
            if(FD_ISSET(clients[i], &rfds)){

                memset(buff[i], 0, sizeof(buff[i]));
                int rsize = recv(clients[i], buff[i], 1024, 0);
                if(rsize <= 0){
                    close(clients[i]);
                    clients[i] = -1;
                }else{
                    char s[1040] = "sxg said : \0";
                    task_queue_push(tq, buff[i]);
                    for(int j = 0; j < rsize; ++j){
                        buff[i][j] = toupper(buff[i][j]);
                    }
                    strcat(s,buff[i]);
                    write(clients[i],s,rsize+12);
                }
                ret--;
                if(ret == 0) break;
            }
        }

    }


    return 0;
}

/*************************************************************************
	> File Name: test.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Thu 02 Mar 2023 03:28:59 PM CST
 ************************************************************************/

#include "head.h"

#define INS 5
#define MAX 100


int main(int argc, char *argv[]){
    DBG(RED"sxgyyds\n"NONE);
    DBG(GRAY"sxgmoney\n"NONE);
    if(argc < 2){
        DBG(RED"fail"NONE);
        return -1;
    }
    FILE *fp;
    pthread_t tid[INS];
    char buff[MAX][1024];
    TQ tq = (TQ)malloc(sizeof(struct task_queue));

    task_queue_init(tq, MAX);

    for(int i = 0; i < INS; i++){
        pthread_create(&tid[i], NULL, thread_run, (void *)tq);
        printf("%dth thread : %lu\n", i, tid[i]);
    }


    while(1){
        int sub = 0;
        if((fp = fopen(argv[1],"r")) == NULL){
            perror("fopen");
            exit(0);
        }

        while(fgets(buff[sub],1024,fp) != NULL){
            // printf("%s\n", buff[sub]);
            task_queue_push(tq, buff[sub]);
            // sleep(1);
            if(++sub == MAX) sub = 0;
        }
        sleep(3);
    }


    return 0;
}

#include "head.h"

#define MAX 100
#define INS 4

char buff[MAX][1024];

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage : %s+fileName\n", argv[0]);
        return 1;
    }
    FILE *fp;
    // 初始化 线程、结构体
    pthread_t tid[INS];
    TQ tq = (TQ)malloc(sizeof(struct task_queue));
    task_queue_init(tq, MAX);
    for(int i = 0; i < INS; i++)
    pthread_create(&tid[i], NULL, run_thread_pool, (void*)tq);
    printf("before while \n");
    while(1){
        int sub = 0;
        // 打开文件 循环读取，放入队列
        if((fp = fopen(argv[1],"r")) == NULL){
            perror("fopen");
            return -1;
        }
        while(fgets(buff[sub],1024,fp) != NULL){
            // printf("buff[sub] = [%s]\n", buff[sub]);
            task_queue_push(tq,buff[sub]);
            if(++sub == MAX) sub = 0;
            // sleep(1);
        }
        fclose(fp);

    }
    return 0;
}

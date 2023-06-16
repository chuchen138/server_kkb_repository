/*************************************************************************
	> File Name: 3.msg-server.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Sun 12 Mar 2023 05:30:14 PM CST
 ************************************************************************/

#include "2.ipc-msg.h"

int main(){
    key_t ftokRes = ftok(FTOK_FILE, ftok_pro);
    if(ftokRes < 0){
        printf("ftok err\n");
        return 1;
    }

    int msgId = msgget(ftokRes, IPC_CREAT|0777);
    if(msgId < 0){
        perror("msgget fail");
        return 1;
    }

    msginfo receivMsg;
    while(1){
        msgrcv(msgId, &receivMsg, 256, 123, 0);
        printf("receive from client msg:%s\n", receivMsg.content);
        receivMsg.type = 124;
        msgsnd(msgId, &receivMsg, 256, 0);
    }
    
    return 0;
}

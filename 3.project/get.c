#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf 
{
    long mtype;       /* message type, must be > 0 */
    char mtext[256];    /* message data */
};
 
 
int main()
{
    struct msgbuf readBuf;
    key_t key;
 
    key = 10001;
    printf("key=%x\n",key);
 
    int msgId = msgget(key, IPC_CREAT|0777);
    if(msgId == -1 ){
        printf("get que failuer\n");
    }
    memset(&readBuf,0,sizeof(struct msgbuf));
 
    int ret = msgrcv(msgId, &readBuf,sizeof(readBuf.mtext),8,IPC_NOWAIT);//接收
    printf("read from que:%s\n ret = %d\n",readBuf.mtext, ret);
 
    // struct msgbuf sendBuf = {988,"thank you for reach"};
    // msgsnd(msgId,&sendBuf,strlen(sendBuf.mtext),0);//写入
 
    // msgctl(msgId,IPC_RMID,NULL);//删除消息队列对象
 
    return 0;
}
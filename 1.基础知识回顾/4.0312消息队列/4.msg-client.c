#include "2.ipc-msg.h"
#include "string.h"

int main(int argc, char *argv[]) {
    key_t ftokRes = ftok(FTOK_FILE, ftok_pro);
    if (ftokRes < 0) {
        perror("ftok fail");
        return 1;
    }
    char buff[256] = "null";
    if(argc > 1) memcpy(buff, argv[1], sizeof(argv[1]));
    msginfo msginfo2Send;
    msginfo2Send.type = 123;
    sprintf(msginfo2Send.content, "client pid:%d -> said(%s)", getpid(),buff);

    int msgId = msgget(ftokRes, IPC_CREAT | 777);
    if (msgId < 0) {
        perror("msgget fail");
        return 0;
    }

    int res = msgsnd(msgId, &msginfo2Send, 256, 0);
    printf("msgsnd res:%d\n", res);
    res = msgrcv(msgId, &msginfo2Send, 256, 124, 0);
    printf("receive from msgserver:%s\n", msginfo2Send.content);
    printf("msgrcv res:%d\n", res);

    return 0;
}

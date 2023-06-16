#include "head.h"


void *client_work(void *arg){
    int fd = *(int *)arg;
    struct wechat_msg msg;
    while(1){
        memset(&msg, 0, sizeof(msg));
        int ret = recv(fd, &msg,sizeof(msg),0);
        if(ret <= 0){
            fprintf(stderr, RED"<Error>"NONE" : Server is out service.\n");
            exit(1);
        }
        if(msg.type &WECHAT_WALL){
            DBG(BLUE"<Public> %s"NONE" : %s\n",msg.name,msg.msg);
        }else if(msg.type &WECHAT_MSG){
            DBG(GREEN"%s"NONE" : %s\n",msg.name,msg.msg);
        }else if(msg.type &WECHAT_SYS){
            DBG(GRAY"Server "NONE": %s\n", msg.msg);
        }
    }
}


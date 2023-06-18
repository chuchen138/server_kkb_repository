#include<stdio.h>
#include<unistd.h>
#include "UserManager.h"
#include "PhotoManager.h"
#include "MessageManager.h"
#include "RelationManager.h"

UserManager user_svr;
RelationManager relation_svr;
MessageManager message_svr;
PhotoManager photo_svr;

void Test(){
    char user_name[] = "sunxiangguo";
    char password[] = "1234456";
    int ret = user_svr.CreateUser(1,user_name,password);
    printf("ret = %d\n",ret);
    ret = user_svr.CreateUser(1,user_name,password);
    printf("ret = %d\n",ret);
}
int main(){
    user_svr.Start();
    relation_svr.Start();
    message_svr.Start();
    photo_svr.Start();

    Test();
    while(1){
        user_svr.Proc();
        relation_svr.Proc();
        message_svr.Proc();
        photo_svr.Proc();
        sleep(1);
    }
    user_svr.Shutdown();
    relation_svr.Shutdown();
    message_svr.Shutdown();
    photo_svr.Shutdown();
    
    return 0;
}

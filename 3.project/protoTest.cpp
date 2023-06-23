#include <stdio.h>
#include <string.h>
#include "pb/message_define.pb.h"

int main(){
    sxg::RegReq reg;
    reg.set_ver(1);
    reg.set_mess_type(401);
    reg.set_user_name("sun");
    reg.set_password("123456");

    char data[256];
    memset(data,0,sizeof(data));
    reg.SerializeToArray(data,256);
    printf("data:[ %s]\n",data);
    for(int i = 0; i < strlen(data);++i){
        printf("%x %c\n",data[i],data[i]);
    }
    

    return 0;
}

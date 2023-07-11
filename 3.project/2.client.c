#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "common/mess_type.h"
#include "pb/message_define.pb.h"
#include <iostream>
using namespace std;

sxg::RegReq regReq1;
sxg::RegRsp regRsp1;
sxg::LoginReq loginReq1;
sxg::LoginRsp loginRsp1;
sxg::LogoutReq logoutReq;
sxg::LogoutRsp logoutRsp;
sxg::RegReq regReq2;
sxg::RegRsp regRsp2;
sxg::LoginReq loginReq2;
sxg::LoginRsp loginRsp2;
sxg::AddFriendReq addFriendReq1;
sxg::AddFriendRsp addFriendRsp1;
sxg::PublishMessageReq publishReq1;
sxg::PublishMessageRsp publishRsp1;
sxg::GetPhotoReq getPhotoReq2;
sxg::GetPhotoRsp getPhotoRsp2;
sxg::GetMessageReq msgGetMessageReq;
sxg::GetMessageRsp msgGetMessageRsp;
sxg::DelFriendReq delFriendReq;
sxg::DelFriendRsp delFriendRsp;
sxg::SignOutReq signoutReq;
sxg::SignOutRsp signoutRsp;

int ProtoInit(){
    regReq1.set_ver(1);
    regReq1.set_mess_type(401);
    regReq1.set_user_name("hank1");
    regReq1.set_password("12345678");

    regReq2.set_ver(1);
    regReq2.set_mess_type(401);
    regReq2.set_user_name("hank2");
    regReq2.set_password("12345678");

    loginReq1.set_ver(1);
    loginReq1.set_mess_type(101);
    loginReq1.set_user_name("hank1");
    loginReq1.set_password("12345678");

    loginReq2.set_ver(1);
    loginReq2.set_mess_type(101);
    loginReq2.set_user_name("hank2");
    loginReq2.set_password("12345678");

    addFriendReq1.set_ver(1);
    addFriendReq1.set_mess_type(301);
    addFriendReq1.set_user_id(2);
    addFriendReq1.set_other_id(1);

    publishReq1.set_user_id(2);
    publishReq1.set_mess_type(201);
    publishReq1.set_content("sxg is hansome!");
    publishReq1.set_sublisher("hank1");
    
    logoutReq.set_user_id(2);
    logoutReq.set_ver(1);

    msgGetMessageReq.set_user_id(1);
    msgGetMessageReq.set_ver(1);

    getPhotoReq2.set_user_id(1);
    getPhotoReq2.set_ver(1);

    return 0;

}


unsigned char send_buffer[10244];
unsigned char recv_buffer[10244];


int SetReqMessType(const char * mess_type){
    send_buffer[0]=mess_type[0];
    send_buffer[1]=mess_type[1];
    send_buffer[2]=mess_type[2];
    return 0;
}
int sock;

// 注销
int SendSignoutReq(){
    signoutReq.set_user_id(1);
    signoutReq.set_ver(1);
    SetReqMessType("501");
    signoutReq.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    return ret;
}

int SendDelFriendReq(){
    delFriendReq.set_user_id(1);
    delFriendReq.set_other_id(2);
    delFriendReq.set_ver(1);
    SetReqMessType("303");
    delFriendReq.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    return 0;
}

int SendGetPhotoReq2(){
    SetReqMessType("203");
    getPhotoReq2.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
int SendRegReq1(){
    SetReqMessType("401");
    regReq1.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
int SendRegReq2(){
    SetReqMessType("401");
    regReq2.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}

int SendLoginReq1(){
    SetReqMessType("101");
    loginReq1.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}

int SendLoginReq2(){
    SetReqMessType("101");
    loginReq2.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}

int SendAddFriendReq1(){
    SetReqMessType("301");
    addFriendReq1.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}

int SendPublishMessageReq1(){
    SetReqMessType("201");
    publishReq1.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}

int SendLogoutReq(){
    SetReqMessType("103");
    logoutReq.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}

int SendGetmessageReq(){
    SetReqMessType("205");
    msgGetMessageReq.SerializeToArray(send_buffer+3,10240);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}

int mess_type;
int GetMessType(){
    return ( (recv_buffer[0]-'0')*100+(recv_buffer[1]-'0')*10 + recv_buffer[2]-'0');
}

int RecvRsp(){
    int ret=recv(sock,recv_buffer,10244,0);
    if(ret>0){
        int mess_type=GetMessType();
        if(mess_type<0){
            return -1;
        }
        switch(mess_type){
            case PUBLISH_MESSAGE_RSP:
            publishRsp1.ParseFromArray(recv_buffer+3,10240);
            printf("Received PublishMessageRsp:\n");
            printf("ret      : %d\n", publishRsp1.ret());
            break;
            case ADD_FRIEND_RSP:
            addFriendRsp1.ParseFromArray(recv_buffer+3,10240);
            printf("Received AddFriendRsp:\n");
            printf("ret      : %d\n", addFriendRsp1.ret());
            break;
            case LOGIN_RSP:
            loginRsp1.ParseFromArray(recv_buffer+3,10240);
            printf("Received LoginRsp:\n");
            printf("ret	 : %d\n", loginRsp1.ret());
            printf("user_id : %d\n", loginRsp1.user_id());
            break;
            case REG_RSP:
            regRsp1.ParseFromArray(recv_buffer+3,10240);
            printf("Received RegRsp:\n");
            printf("ret	 : %d\n", regRsp1.ret());
            printf("user_id : %d\n", regRsp1.user_id());
            break;
            case LOGOUT_RSP:
            logoutRsp.ParseFromArray(recv_buffer+3,10240);
            printf("Received LogoutRsp:\n");
            break;
            case GET_MESSAGE_LIST_RSP:
            msgGetMessageRsp.ParseFromArray(recv_buffer+3,10240);
            {
                for(int i = 0; i < msgGetMessageRsp.message_list_size(); i++){
                    sxg::MessageInfoBase mess_base_tmp = msgGetMessageRsp.message_list(i);
                    cout << "!!! recv nice : " << mess_base_tmp.content() << endl;
                }
                
            }
            break;
            case GET_PHOTO_RSP:
                getPhotoRsp2.ParseFromArray(recv_buffer+3,10240);
                printf("Received %s GET_PHOTO_RSP: ret = %d, message_id = %d, publish_time = %d\n",\
                    getPhotoRsp2.publisher().c_str(), getPhotoRsp2.ret(), getPhotoRsp2.message_id(),\
                        getPhotoRsp2.publish_time());
            break;
            case DEL_FRIEND_RSP:
                delFriendRsp.ParseFromArray(recv_buffer+3,10240);
                printf("Received DEL_FRIEND_RSP: ret = %d\n", delFriendRsp.ret());
            break;
            case SIGN_OUT_RSP:
                signoutRsp.ParseFromArray(recv_buffer+3,10240);
                printf("Received SIGN_OUT_RSP: ret = %d\n", signoutRsp.ret());
                break;
        }
        return 0;
    }

    return -1;
}

int ClientSocketInit(){
    sock=socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0 ,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8999);
    int conn_ret=connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(conn_ret<0){
        printf("connect_ret:%d\n",conn_ret);
        return conn_ret;
    }
    return 0;		
}
//
//1.reg hank1
//2.login hank1
//3.reg hank2
//4.login hank2
//5.hank1 add hank2
//6.hank1 publish message
//7.hank2 get photo

int main(){
    int client_on=0;
    int n=0;
    int ret;
    ClientSocketInit();
    ProtoInit();

// 注册成员
// SendRegReq1();
// RecvRsp();
// SendRegReq2();
// RecvRsp();

// 添加朋友
// SendAddFriendReq1();
// RecvRsp();

// 删除朋友
// SendDelFriendReq();
// RecvRsp();

// 注销
// SendSignoutReq();
// RecvRsp();
    // 发送消息
    // SendLoginReq2();
    // RecvRsp();
    // SendPublishMessageReq1();
    // RecvRsp();

    // 获取消息列表
    SendLoginReq1();
    RecvRsp();
    SendGetPhotoReq2();
    // SendGetmessageReq();
    RecvRsp();
    while(client_on){
        printf("please input the number:\n");
        scanf("%d",&n);
        switch(n){
            case 0:
            ProtoInit();
            n=1;
            break;
            case 1:
            ret=SendRegReq1();
            if(ret==0){
                n++;
            }
            break;
            case 2:
            ret=RecvRsp();
            if(ret==0){
                n++;
            }
            break;
            case 3:
            ret=SendRegReq2();
            if(ret==0){
                n++;
            }
            break;
            case 4:
            ret=RecvRsp();
            if(ret==0){
                n++;
            }
            break;
            case 5:
            ret=SendAddFriendReq1();
            break;
            case 6:
            ret=RecvRsp();
            if(ret==0){
                n++;
            }
            break;
            case 7:
            ret=SendPublishMessageReq1();
            if(ret==0){
                n++;
            }
            break;
            case 8:
            ret=RecvRsp();
            if(ret==0){
                n++;
            }
            break;
            case 9:
            ret=SendLoginReq2();
            if(ret==0){
                n++;
            }
            RecvRsp();
            break;
            case 10:
            ret=SendLogoutReq();
            ret=RecvRsp();
            client_on=0;
            close(sock);
            case 11:
            ret=SendGetmessageReq();
            if(ret==0){
                n++;
            }
            break;
            case 12:
            ret=RecvRsp();
            if(ret==0){
                n++;
            }
            break;
            case 13:
            ret=SendLoginReq1();
            if(ret==0){
                n++;
            }
            RecvRsp();
            break;
            default:
                //n=0;
                client_on=0;
                break;
        }
        }
        return 0;
}


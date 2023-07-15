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

sxg::RegReq regReq;
sxg::RegRsp regRsp;
sxg::LoginReq loginReq;
sxg::LoginRsp loginRsp;
sxg::AddFriendReq addFriendReq;
sxg::AddFriendRsp addFriendRsp;
sxg::DelFriendReq delFriendReq;
sxg::DelFriendRsp delFriendRsp;
sxg::AddBlackReq msgAddBlackReq;
sxg::AddBlackRsp msgAddBlackRsp;
sxg::DelBlackReq msgDelBlackReq;
sxg::DelBlackRsp msgDelBlackRsp;
sxg::PublishMessageReq publishReq;
sxg::PublishMessageRsp publishRsp;
sxg::GetPhotoReq getPhotoReq;
sxg::GetPhotoRsp getPhotoRsp;
sxg::GetMessageReq msgGetMessageReq;
sxg::GetMessageRsp msgGetMessageRsp;
sxg::LogoutReq logoutReq;
sxg::LogoutRsp logoutRsp;
sxg::SignOutReq signoutReq;
sxg::SignOutRsp signoutRsp;

unsigned char send_buffer[4096];
unsigned char recv_buffer[4096];


int SetReqMessType(const char * mess_type){
    send_buffer[0]=mess_type[0];
    send_buffer[1]=mess_type[1];
    send_buffer[2]=mess_type[2];
    return 0;
}
int sock, user_id;
char user_name[20];
char password[20];
char other_name[20];
char content[100];

// 注册
int SendRegReq(){
    regReq.set_ver(1);
    regReq.set_mess_type(401);
    printf("设置用户名:\n");
    scanf("%s",user_name);
    printf("设置密码:\n");
    scanf("%s",password);
    regReq.set_user_name(user_name);
    regReq.set_password(password);
    SetReqMessType("401");
    regReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 登录
int SendLoginReq(){
    loginReq.set_ver(1);
    loginReq.set_mess_type(101);
    printf("设置用户名:\n");
    scanf("%s",user_name);
    printf("设置密码:\n");
    scanf("%s",password);
    loginReq.set_user_name(user_name);
    loginReq.set_password(password);
    SetReqMessType("101");
    loginReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 添加好友
int SendAddFriendReq(){
    memset(other_name,0,sizeof(other_name));
    printf("设置添加好友的姓名:\n");
    scanf("%s",other_name);
    addFriendReq.set_other_name(other_name);
    addFriendReq.set_ver(1);
    addFriendReq.set_mess_type(301);
    addFriendReq.set_user_id(user_id);
    SetReqMessType("301");
    addFriendReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 删除好友
int SendDelFriendReq(){
    memset(other_name,0,sizeof(other_name));
    printf("设置删除好友的姓名:\n");
    scanf("%s",other_name);
    addFriendReq.set_other_name(other_name);
    delFriendReq.set_ver(1);
    delFriendReq.set_mess_type(303);
    delFriendReq.set_user_id(1);
    SetReqMessType("303");
    delFriendReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    return 0;
}
// 添加黑名单
int SendAddBlackReq(){
    memset(other_name,0,sizeof(other_name));
    printf("设置添加黑名单的好友姓名:\n");
    scanf("%s",other_name);
    addFriendReq.set_other_name(other_name);
    msgAddBlackReq.set_ver(1);
    msgAddBlackReq.set_mess_type(305);
    msgAddBlackReq.set_user_id(user_id);
    SetReqMessType("305");
    msgAddBlackReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 删除黑名单
int SendDelBlackReq(){
    memset(other_name,0,sizeof(other_name));
    printf("设置删除默认名单的好友姓名:\n");
    scanf("%s",other_name);
    addFriendReq.set_other_name(other_name);
    msgDelBlackReq.set_ver(1);
    msgDelBlackReq.set_mess_type(307);
    msgDelBlackReq.set_user_id(user_id);
    SetReqMessType("307");
    msgDelBlackReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 发布消息
int SendPublishMessageReq(){
    memset(content,0,sizeof(content));
    memset(other_name,0,sizeof(other_name));
    printf("输入你要发送的好友姓名:\n");
    scanf("%s",other_name);
    printf("输入内容:\n");
    scanf("%s",content);
    publishReq.set_user_id(user_id);
    publishReq.set_mess_type(201);
    publishReq.set_content(content);
    publishReq.set_sublisher(other_name);
    SetReqMessType("201");
    publishReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 拉取快照
int SendGetPhotoReq(){
    getPhotoReq.set_ver(1);
    getPhotoReq.set_mess_type(203);
    getPhotoReq.set_user_id(user_id);
    SetReqMessType("203");
    getPhotoReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 获取信息
int SendGetmessageReq(){
    msgGetMessageReq.set_ver(1);
    msgGetMessageReq.set_mess_type(205);
    msgGetMessageReq.set_user_id(user_id);
    SetReqMessType("205");
    msgGetMessageReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 退出登录
int SendLogoutReq(){
    logoutReq.set_ver(1);
    logoutReq.set_mess_type(103);
    logoutReq.set_user_id(user_id);
    SetReqMessType("103");
    logoutReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    if(ret>0){
        return 0;
    }
    return -1;
}
// 注销
int SendSignoutReq(){
    signoutReq.set_ver(1);
    // signoutReq.set_mess_type(501);
    signoutReq.set_user_id(user_id);
    SetReqMessType("501");
    signoutReq.SerializeToArray(send_buffer+3,4090);
    int size_buffer=sizeof(send_buffer);
    int ret=send(sock, send_buffer, size_buffer, 0);
    return ret;
}


int mess_type;
int GetMessType(){
    return ( (recv_buffer[0]-'0')*100+(recv_buffer[1]-'0')*10 + recv_buffer[2]-'0');
}

int RecvRsp(){
    int ret=recv(sock,recv_buffer,4096,0);
    if(ret>0){
        int mess_type=GetMessType();
        printf("recv_buffer: [%s]\n", recv_buffer);
        if(mess_type<0){
            return -1;
        }
        switch(mess_type){
            case REG_RSP:
                regRsp.ParseFromArray(recv_buffer+3,4090);
                user_id = regRsp.user_id();
                printf("Received RegRsp:\n");
                printf("ret	 : %d\n", regRsp.ret());
                printf("user_id : %d\n", user_id);
            break;
            case LOGIN_RSP:
                loginRsp.ParseFromArray(recv_buffer+3,4090);
                user_id = loginRsp.user_id();
                printf("Received LoginRsp:\n");
                printf("ret	 : %d\n", loginRsp.ret());
                printf("user_id : %d\n", user_id);
            break;
            case ADD_FRIEND_RSP:
                addFriendRsp.ParseFromArray(recv_buffer+3,4090);
                printf("Received AddFriendRsp:\n");
                printf("ret      : %d\n", addFriendRsp.ret());
            break;
            case DEL_FRIEND_RSP:
                delFriendRsp.ParseFromArray(recv_buffer+3,4090);
                printf("Received DEL_FRIEND_RSP: ret = %d\n", delFriendRsp.ret());
            break;
            case ADD_BLACK_RSP:
                msgAddBlackRsp.ParseFromArray(recv_buffer+3,4090);
                printf("Received ADD_BLACK_RSP: ret = %d\n", msgAddBlackRsp.ret());
            break;
            case DEL_BLACK_RSP:
                msgDelBlackRsp.ParseFromArray(recv_buffer+3,4090);
                printf("Received DEL_BLACK_RSP: ret = %d\n", msgDelBlackRsp.ret());
            break;
            case PUBLISH_MESSAGE_RSP:
                publishRsp.ParseFromArray(recv_buffer+3,4090);
                printf("Received PublishMessageRsp:\n");
                printf("ret      : %d\n", publishRsp.ret());
            break;
            case GET_MESSAGE_LIST_RSP:
            msgGetMessageRsp.ParseFromArray(recv_buffer+3,4090);
            {
                for(int i = 0; i < msgGetMessageRsp.message_list_size(); i++){
                    sxg::MessageInfoBase mess_base_tmp = msgGetMessageRsp.message_list(i);
                    cout << "!!! recv nice : " << mess_base_tmp.content() << endl;
                }
            }
            break;
            case GET_PHOTO_RSP:
                getPhotoRsp.ParseFromArray(recv_buffer+3,4090);
                printf("Received %s GET_PHOTO_RSP: ret = %d, message_id = %d, publish_time = %d\n",\
                    getPhotoRsp.publisher().c_str(), getPhotoRsp.ret(), getPhotoRsp.message_id(),\
                        getPhotoRsp.publish_time());
            break;
            case LOGOUT_RSP:
                logoutRsp.ParseFromArray(recv_buffer+3,4090);
                printf("Received LogoutRsp:\n");
            break;
            case SIGN_OUT_RSP:
                signoutRsp.ParseFromArray(recv_buffer+3,4090);
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

#define MESSAGE_WARN "输入消息序号\n\
//1.注册 \n\
//2.登录  \n\
//3.添加好友 \n\
//4.删除好友 \n\
//5.添加黑名单 \n\
//6.删除黑名单 \n\
//7.发送消息 \n\
//8.拉取快照 \n\
//9.获取消息列表 \n\
//10.退出 \n\
//11.注销 \n"

int main(){
    int client_on=1;
    int n=0;
    int ret;
    ClientSocketInit();
    printf(MESSAGE_WARN);
    while(client_on){
        memset(recv_buffer, 0, sizeof(recv_buffer));
        memset(send_buffer, 0, sizeof(send_buffer));
        printf("please input the number:\n");
        scanf("%d",&n);
        switch(n){
            case 1:
                SendRegReq();
                RecvRsp();
            break;
            case 2:
                ret=SendLoginReq();
                RecvRsp();
            break;
            case 3:
                ret=SendAddFriendReq();
                RecvRsp();
            break;
            case 4:
                ret=SendDelFriendReq();
                RecvRsp();
            break;
            case 5:
                ret=SendAddBlackReq();
                RecvRsp();
            break;
            case 6:
                ret=SendDelBlackReq();
                RecvRsp();
            break;
            case 7:
                ret=SendPublishMessageReq();
                RecvRsp();
            break;
            case 8:
                ret=SendGetPhotoReq();
                RecvRsp();
            break;
            case 9:
                ret=SendGetmessageReq();
                RecvRsp();
            break;
            case 10:
                ret=SendLogoutReq();
                RecvRsp();
            break;
            case 11:
                ret=SendSignoutReq();
                RecvRsp();
            break;
            case 12:
            ret=RecvRsp();
            break;
            case 13:
            break;
            default:
                printf("please input the correct number!\n");
                break;
        }
        // printf("ret = %d\n", ret);
    }
    
    close(sock);
    return 0;
}


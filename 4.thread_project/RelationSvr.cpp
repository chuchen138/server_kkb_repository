#include <stdio.h>
#include <unistd.h>

#include <sys/time.h>
#include "common/ret_value.h"
#include "common/mess_type.h"
#include "UserManager.h"
#include "RelationManager.h"
#include "MessageManager.h"
#include "PhotoManager.h"
#include "pb/message_define.pb.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


RelationManager rela_svr;
DbManager db_svr;
int debug_on = 1;


int mess_type = -1;
int server_sock;
struct sockaddr_in server_addr;


int no_recv_count=0;
struct sockaddr_in clnt_addr;
int clnt_sock;
int accept_flag=0;
int send_fail_count = 0;


sxg::RegReq msgRegReq;
sxg::RegRsp msgRegRsp;

sxg::LoginReq msgLoginReq;
sxg::LoginRsp msgLoginRsp;

sxg::LogoutReq logoutReq;
sxg::LogoutRsp logoutRsp;

sxg::PublishMessageReq msgPublishMessageReq;
sxg::PublishMessageRsp msgPublishMessageRsp;

sxg::GetMessageReq msgGetMessageReq;
sxg::GetMessageRsp msgGetMessageRsp;

sxg::GetPhotoReq msgGetPhotoReq;
sxg::GetPhotoRsp msgGetPhotoRsp;

sxg::AddFriendReq msgAddFriendReq;
sxg::AddFriendRsp msgAddFriendRsp;

sxg::DelFriendReq msgDelFriendReq;
sxg::DelFriendRsp msgDelFriendRsp;

sxg::AddBlackReq msgAddBlackReq;
sxg::AddBlackRsp msgAddBlackRsp;

sxg::DelBlackReq msgDelBlackReq;
sxg::DelBlackRsp msgDelBlackRsp;

char mess_type_str[10];

char *shmp_user_rela;
char *shmp_rela_user;
char *shmp_user_mess;
char *shmp_mess_user;
char *shmp_photo_user;
char *shmp_user_photo;
int ShmInit(){
	key_t shmkey = 9999;
	int shm_id = shmget(shmkey,10240*12,IPC_CREAT|0666);
	if(shm_id < 0) return -1;
	char *shmp = shmat(shm_id,NULL,0);
	if(shmp < 0) return -2;
	// use -> rela
	shmp_user_rela = shmp;
	// shmp_user_mess = shmp+10240;
	// shmp_user_photo = shmp+20480;

	shmp_rela_user = shmp+10240*6;
	// shmp_mess_user = shmp+10240*7;
	// shmp_photo_user = shmp+10240*8;
}

int BUSCheckRecv(){
	if(*shmp_user_rela){
		mess_type = (shmp_user_rela[1]-'0')*100 + (shmp_user_rela[2]-'0')*10 + (shmp_user_rela[3]-'0');
		return 1;
	}
	return 0;
}

int RecvBusRelationUser(sxg::RelationInfoBase* rela_info_base){
	switch(mess_type){
		case RELATION_INIT:
			rela_info_base->ParseFromArray(shmp_rela_user+4,10240);
			break;
	}
}

int SetBusMessType(int mess_type_req){
	char mess_type_str[256];
	sprintf(mess_type_str,"%d",mess_type_req);
	int svr_type = mess_type_req/100;
	if(svr_type == USER_RELA_BUS_TYPE){
		if(shmp_user_rela[0] != 0){
			return -USER_RELA_BUS_TYPE;
		}
		shmp_user_rela[1] = mess_type_str[0];
		shmp_user_rela[2] = mess_type_str[1];
		shmp_user_rela[3] = mess_type_str[2];
	}else if(svr_type == USER_MESS_BUS_TYPE){
		if(shmp_user_mess[0] != 0){
			return -USER_MESS_BUS_TYPE;
		}
		shmp_user_mess[1] = mess_type_str[0];
		shmp_user_mess[2] = mess_type_str[1];
		shmp_user_mess[3] = mess_type_str[2];
	}else if(svr_type == USER_PHOTO_BUS_TYPE){
		if(shmp_user_photo[0] != 0){
			return -USER_PHOTO_BUS_TYPE;
		}
		shmp_user_photo[1] = mess_type_str[0];
		shmp_user_photo[2] = mess_type_str[1];
		shmp_user_photo[3] = mess_type_str[2];
	}
	return 0;
}
int SendBusUser(){
	shmp_rela_user[0]='A';
}
int main() {

    db_svr.Init();
    rela_svr.Init(&db_svr);
	// user_svr.Start();
	rela_svr.Start();

	// Test();
	// SocketInit();
	// SocketAccept();
	// int times=0;
	int server_on=1;
	while(server_on){
		int now = time(NULL);
		// SocketCheckRecv();
		BUSCheckRecv();
		int ret=0;
		if(mess_type<0){
			usleep(5000);
			continue;
		}
		// printf("message count: %d\n",times);fflush(stdout);
		switch(mess_type){
			case RELATION_INIT:
				sxg::UserInfoBase user_rela;
				user_rela.ParseFromArray(shmp_user_rela+4,10240);
				memset(shmp_user_rela+4,0,10240);
				RelationInfo *rela_info = rela_svr.GetRelation(user_rela.user_id());
				if(rela_info){
					rela_user.set_user_id(user_rela.user_id());
				}else{
					rela_svr.UserRelationInit(user_rela.user_id());
					rela_user.set_user_id(USER_NOT_EXIST);
				}
				SetBusMessType(RELATION_INIT);
				rela_user.SerializeToArray(shmp_rela_user);
				break;
			
			case ADD_FRIEND_REQ:
				msgAddFriendReq.ParseFromArray(recv_buffer+3,10240);
				printf("301 req{uid1:%d uid2:%d}\n",msgAddFriendReq.user_id(),msgAddFriendReq.other_id());
				ret = user_svr.CheckExist(msgAddFriendReq.user_id());
				if(ret==USER_EXIST){
					ret=user_svr.CheckExist(msgAddFriendReq.other_id());
					if(ret==USER_EXIST){
						ret=rela_svr.AddFriend(	msgAddFriendReq.user_id(),
												msgAddFriendReq.other_id());
												msgAddFriendRsp.set_ret(ret);
					}
					msgAddFriendRsp.set_ret(ret);
				}else{
					msgAddFriendRsp.set_ret(ret);
				}
				printf("302 msgAddFriendRsp ret:%d\n",ret);fflush(stdout);
				sprintf(mess_type_str,"%d",ADD_FRIEND_RSP);
                SetRspMessType(mess_type_str);
                msgLoginRsp.SerializeToArray(send_buffer+3,10240);
				if(debug_on)printf("[DEBUG   ]ret_byte_size:%d\n",ret);
                SocketSendRsp(msgAddFriendRsp.ByteSize()+3);
                printf("..\n");
				break;
            case DEL_FRIEND_REQ:
				msgDelFriendReq.ParseFromArray(recv_buffer+3,10240);
				if(debug_on)printf("[DEBUG   ]mess_type:303, uid1:%d uid2:%d\n",msgDelFriendReq.user_id(),msgDelFriendReq.other_id());
				ret = user_svr.CheckExist(msgDelFriendReq.user_id());
				if(ret==USER_EXIST){
					ret=user_svr.CheckExist(msgDelFriendReq.other_id());
					if(ret==USER_EXIST){
						ret=rela_svr.DeleteFriend(	msgDelFriendReq.user_id(),
									msgDelFriendReq.other_id());
						msgDelFriendRsp.set_ret(ret);
					}else{
						msgDelFriendRsp.set_ret(ret);
					}
				}else{
					msgDelFriendRsp.set_ret(ret);
				}
				if(debug_on)printf("[DEBUG   ]304 DelFriendRsp ret:%d\n",ret);fflush(stdout);
				sprintf(mess_type_str,"%d",DEL_FRIEND_RSP);
				SetRspMessType(mess_type_str);
				ret=msgDelFriendRsp.SerializeToArray(send_buffer+3,10240);
				if(debug_on)printf("[DEBUG   ]ret_byte_size:%d\n",ret);
				SocketSendRsp(msgDelFriendRsp.ByteSize()+3);
                printf("..\n");
                break;
				case ADD_BLACK_REQ:
					msgAddBlackReq.ParseFromArray(recv_buffer+3,10240);
					ret = user_svr.CheckExist(msgAddBlackReq.user_id());
					if(ret==USER_EXIST){
						ret=user_svr.CheckExist(msgAddBlackReq.other_id());
						if(ret==USER_EXIST){
							ret=rela_svr.AddBlack(	msgAddBlackReq.user_id(),
										msgAddBlackReq.other_id());
										msgAddBlackRsp.set_ret(ret);
						}
						msgAddBlackRsp.set_ret(ret);
					}else{
						msgAddBlackRsp.set_ret(ret);
					}
					msgAddBlackRsp.set_ret(ret);
                    sprintf(mess_type_str,"%d",mess_type+1);
                    SetRspMessType(mess_type_str);
					msgAddBlackRsp.SerializeToArray(send_buffer+3,10240);
                    SocketSendRsp(msgAddBlackRsp.ByteSize());
                printf("..\n");
				break;
				case PUBLISH_MESSAGE_REQ:
				msgPublishMessageReq.ParseFromArray(recv_buffer+3,10240);
				ret = user_svr.CheckExist(msgPublishMessageReq.user_id());
				if(ret==USER_EXIST){
					MessageInfo message;
					message.set_message_id(1);/* todo get from mysql */
					message.set_publisher(msgPublishMessageReq.user_id());
					message.set_publish_time(now);
					ret = mess_svr.PublishMessage(message);
					msgPublishMessageRsp.set_ret(ret);
				}else{
					msgPublishMessageRsp.set_ret(ret);
				}
                sprintf(mess_type_str,"%d",mess_type+1);
                SetRspMessType(mess_type_str);
				msgPublishMessageRsp.SerializeToArray(send_buffer+3,10240);
                SocketSendRsp(msgPublishMessageRsp.ByteSize());
                printf("..\n");
				break;
			default:
				printf("unknown mess_type: %d\n",mess_type);
				break;
		}
		user_svr.Proc();
		rela_svr.Proc();
		mess_svr.Proc();
		photo_svr.Proc();
		mess_type=-1;
		usleep(5000);/* 5ms */
	}

	SocketClose();
	photo_svr.Shutdown();
	mess_svr.Shutdown();
	user_svr.Shutdown();
	rela_svr.Shutdown();

	return 0;
}


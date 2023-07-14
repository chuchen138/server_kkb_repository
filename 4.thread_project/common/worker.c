/*************************************************************************
	> File Name: worker.c
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Tue 11 Jul 2023 11:38:04 PM CST
 ************************************************************************/

#include "worker.h"

extern UserManager user_svr;
extern RelationManager rela_svr;
extern MessageManager mess_svr;
extern PhotoManager photo_svr;
extern DbManager db_svr;
extern BusManager bus_svr;

static int SetRspMessType(char *mess_type, char *send_buffer){
	if(strlen(mess_type)>=3){
		send_buffer[0]=mess_type[0];
		send_buffer[1]=mess_type[1];
		send_buffer[2]=mess_type[2];
	}
	return 0;
}

static int SocketSendRsp(int fd, int send_size, char *send_buffer){
	int ret=send(fd, send_buffer, send_size, 0);
	/*printf("send_ret:%d %d\n",ret,send_size);*/
	if(ret<0){
		DBG(RED "fd = (%d)write fail.\n" NONE,fd);
	}
	return 0;
}

void *thread_work(void *arg){
    pthread_detach(pthread_self());
	sxg::RegReq msgRegReq;
	sxg::RegRsp msgRegRsp;

	sxg::LoginReq msgLoginReq;
	sxg::LoginRsp msgLoginRsp;

	sxg::LogoutReq logoutReq;
	sxg::LogoutRsp logoutRsp;

	sxg::SignOutReq signoutReq;
	sxg::SignOutRsp signoutRsp;

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
	char send_buffer[4100];
	char mess_type_str[32];
	const int debug_on = 1;
	int ret = 0;
    TQ tq = (TQ)arg;
    DBG(GREEN "thread_pid = %ld,work begin\n" NONE,pthread_self());
    printf(GREEN "print thread_pid = %ld,work begin\n" NONE,pthread_self());

    while(1){
		int fd;
		int now = time(NULL);
		char *recv_buffer= (char *)task_queue_pop(tq, &fd);
		int mess_type = (recv_buffer[0]-'0')*100+(recv_buffer[1]-'0')*10+recv_buffer[2]-'0';
		// DBG(GREEN "thread_pid = %ld,recv message : %d\n" NONE,pthread_self(),mess_type);
		// printf(RED "print fd = %d thread_pid = %ld,message type : %d\n" NONE,fd,pthread_self(),mess_type);
		// printf(RED "recv_buffer = %s\n" NONE,recv_buffer);
// pthread_mutex_lock(&tq->mutex);
// pthread_mutex_unlock(&tq->mutex);
        switch(mess_type){
			case REG_REQ: // register request;
				msgRegReq.ParseFromArray(recv_buffer+3,10240);
				ret=user_svr.CreateUser(msgRegReq.user_name().c_str(),
							msgRegReq.password().c_str(),
							msgRegReq.from(),now);
				printf("user_name = %s, password = %s, from = %d, time = %d,ret = %d\n",\
							msgRegReq.user_name().c_str(),
							msgRegReq.password().c_str(),
							msgRegReq.from(),now,
							ret 
							);
				msgRegRsp.set_ret(ret);
				if(ret == SUCCESS){
					int register_user_id = user_svr.GetUserIdByUserName(msgRegReq.user_name().c_str());
                	msgRegRsp.set_user_id(register_user_id);
					msgRegRsp.set_user_name(msgRegReq.user_name());
					rela_svr.UserRelationInit(register_user_id);
				}
				sprintf(mess_type_str,"%d",mess_type+1);
                SetRspMessType(mess_type_str, send_buffer);
                ret = msgRegRsp.SerializeToArray(send_buffer+3,10240);
                printf("ret:%d\n",ret);
                SocketSendRsp(fd,msgRegRsp.ByteSize()+3,send_buffer);
				break;
			case LOGIN_REQ:
				msgLoginReq.ParseFromArray(recv_buffer+3,10240);
                printf("login uname : %s\n",msgLoginReq.user_name().c_str());
				ret=user_svr.LoginCheck(msgLoginReq.user_name().c_str(),
							msgLoginReq.password().c_str());
				if(ret==SUCCESS){
					/*update login time;*/
					// printf(GREEN "user_name = %s, password = %s, time = %d,ret = %d\n" NONE,\
					// msgLoginReq.user_name().c_str(),msgLoginReq.password().c_str(),now,ret);
					int user_id=user_svr.GetUserIdByUserName(msgLoginReq.user_name().c_str());
					user_svr.UpdateUserLoginTime(user_id, now);
					msgLoginRsp.set_user_id(user_id);
				}else{
					// printf(RED "login fail, ret = %d\n" NONE,ret);
					msgLoginRsp.set_ret(ret);
                    msgLoginRsp.set_user_id(0);
				}
				msgLoginRsp.set_ret(ret);
				sprintf(mess_type_str,"%d",mess_type+1);
                SetRspMessType(mess_type_str, send_buffer);
                ret = msgLoginRsp.SerializeToArray(send_buffer+3,4090);
				printf(GREEN "user_id = %d, ret = %d, msgRegRsp.ByteSize() = %d\n" NONE,\
				msgLoginRsp.user_id(),msgLoginRsp.ret(), msgRegRsp.ByteSize());
				printf(GREEN "send:[%s]\n" NONE,send_buffer);
                printf("ret:%d\n",ret);
                // SocketSendRsp(msgLoginRsp.ByteSize()+3);
                SocketSendRsp(fd,msgLoginRsp.ByteSize()+3,send_buffer);
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
                SetRspMessType(mess_type_str, send_buffer);
                msgAddFriendRsp.SerializeToArray(send_buffer+3,10240);
				if(debug_on)printf("[DEBUG   ]ret_byte_size:%d\n",ret);
                SocketSendRsp(fd,msgAddFriendRsp.ByteSize()+3,send_buffer);
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
				SetRspMessType(mess_type_str, send_buffer);
				ret=msgDelFriendRsp.SerializeToArray(send_buffer+3,10240);
				if(debug_on)printf("[DEBUG   ]ret_byte_size:%d\n",ret);
                SocketSendRsp(fd,msgDelFriendRsp.ByteSize()+3,send_buffer);
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
                    SetRspMessType(mess_type_str, send_buffer);
					msgAddBlackRsp.SerializeToArray(send_buffer+3,10240);
                	SocketSendRsp(fd,msgAddBlackRsp.ByteSize()+3,send_buffer);
				break;
				// 删除黑名单
				case DEL_BLACK_REQ:
					msgDelBlackReq.ParseFromArray(recv_buffer+3,10240);
					ret = user_svr.CheckExist(msgDelBlackReq.user_id());
					if(ret==USER_EXIST){
						ret=user_svr.CheckExist(msgDelBlackReq.other_id());
						if(ret==USER_EXIST){
							ret=rela_svr.DeleteBlack(	msgDelBlackReq.user_id(),
										msgDelBlackReq.other_id());
										msgDelBlackRsp.set_ret(ret);
						}
						msgDelBlackRsp.set_ret(ret);
					}else{
						msgDelBlackRsp.set_ret(ret);
					}
					msgDelBlackRsp.set_ret(ret);
                    sprintf(mess_type_str,"%d",mess_type+1);
                    SetRspMessType(mess_type_str, send_buffer);
					msgDelBlackRsp.SerializeToArray(send_buffer+3,10240);
                SocketSendRsp(fd,msgDelBlackRsp.ByteSize()+3,send_buffer);
				break;
				case PUBLISH_MESSAGE_REQ:
				msgPublishMessageReq.ParseFromArray(recv_buffer+3,10240);
				ret = user_svr.CheckExist(msgPublishMessageReq.user_id());
				printf("recv : %s\n",msgPublishMessageReq.content().c_str());
				if(ret==USER_EXIST){
					ret = user_svr.GetUserIdByUserName(msgPublishMessageReq.sublisher().c_str());
					int checkFriendRet = rela_svr.CheckFriend(msgPublishMessageReq.user_id(),ret);
					int checkBlackRet = rela_svr.CheckBlack(msgPublishMessageReq.user_id(),ret);
					if(ret != USER_NOT_EXIST && (checkBlackRet==ALREADY_BLACK || checkFriendRet==ALREADY_FRIEND)){
						MessageInfo message;
						// message.set_message_id(1); /* todo get from mysql */
						message.set_publisher(user_svr.GetUserNameByUserId(msgPublishMessageReq.user_id()));
						message.set_publish_time(now);
						message.set_content(msgPublishMessageReq.content().c_str());
						message.set_sublisher(ret);
						bus_svr.AddBusMessage(message);
						int message_id = mess_svr.PublishMessage(message);
						if(checkFriendRet == ALREADY_FRIEND)
							ret = photo_svr.UpdatePhoto(message.sublisher(),message.publisher()\
							,message.publish_time(),message_id);
					}
					if(checkBlackRet != ALREADY_BLACK && checkFriendRet != ALREADY_FRIEND)
						msgPublishMessageRsp.set_ret(NOT_FRIEND);
					if(ret == USER_NOT_EXIST)
						msgPublishMessageRsp.set_ret(ret);
				}else{
					msgPublishMessageRsp.set_ret(ret);
				}
                sprintf(mess_type_str,"%d",mess_type+1);
                SetRspMessType(mess_type_str, send_buffer);
				msgPublishMessageRsp.SerializeToArray(send_buffer+3,10240);
                SocketSendRsp(fd,msgPublishMessageRsp.ByteSize()+3,send_buffer);
				break;
				case GET_MESSAGE_LIST_REQ:
					msgGetMessageReq.ParseFromArray(recv_buffer+3,4090);
					ret = bus_svr.GetBusMessage(msgGetMessageReq.user_id(),&msgGetMessageRsp);
					
					// ret = mess_svr.GetMessageList(msgGetMessageReq.user_id());
                	sprintf(mess_type_str,"%d",mess_type+1);
                	SetRspMessType(mess_type_str, send_buffer);
					msgGetMessageRsp.SerializeToArray(send_buffer+3,4090);
					photo_svr.UpdatePhoto(msgGetMessageReq.user_id(),"",0,PHOTO_NOT_EXIST);
                SocketSendRsp(fd,msgGetMessageRsp.ByteSize()+3,send_buffer);
				printf("\n send : [%s]\n",send_buffer);
				for(int i = 0; i < msgGetMessageRsp.ByteSize()+3; i++){
					printf("%c",send_buffer[i]);
				}
				putchar(10);
				break;
				case GET_PHOTO_REQ:
					msgGetPhotoReq.ParseFromArray(recv_buffer+3,4090);
					{
						PhotoInfo *photo_info_tmp = photo_svr.GetPhoto(msgGetPhotoReq.user_id());
						if(photo_info_tmp == nullptr) {
							ret = PHOTO_NOT_EXIST;
							msgGetPhotoRsp.set_ret(ret);
						} else {
							ret = SUCCESS;	
							msgGetPhotoRsp.set_ret(ret);
							msgGetPhotoRsp.set_message_id(photo_info_tmp->last_publish_message_id());
							msgGetPhotoRsp.set_publish_time(photo_info_tmp->last_publish_time());
							msgGetPhotoRsp.set_publisher(photo_info_tmp->last_publisher());
							// photo_info_tmp->set_last_publish_message_id(MESSAGE_NOT_EXIST); // 表示没有信息
						}
					}
					
					sprintf(mess_type_str,"%d",mess_type+1);
					SetRspMessType(mess_type_str, send_buffer);
					msgGetPhotoRsp.SerializeToArray(send_buffer+3,10240);
                SocketSendRsp(fd,msgGetPhotoRsp.ByteSize()+3,send_buffer);
				break;
				case LOGOUT_REQ:
				logoutReq.ParseFromArray(recv_buffer+3,10240);
				ret = user_svr.UserLogout(logoutReq.user_id(),now);
				logoutRsp.set_ret(ret);
                printf("ret:%d\n",ret);
				sprintf(mess_type_str,"%d",mess_type+1);
                SetRspMessType(mess_type_str, send_buffer);
                ret = logoutRsp.SerializeToArray(send_buffer+3,10240);
                SocketSendRsp(fd,logoutRsp.ByteSize()+3,send_buffer);
				break;
				case SIGN_OUT_REQ:
					signoutReq.ParseFromArray(recv_buffer+3,10240);
					ret = user_svr.DeleteUser(signoutReq.user_id());
					rela_svr.DeleteRela(signoutReq.user_id());
					db_svr.DeleteMessAndPhoto(signoutReq.user_id());
					bus_svr.DeleteBusMessage(signoutReq.user_id());
					signoutRsp.set_ret(ret);
					
					printf("signoutRsp ret:%d\n",ret);
					sprintf(mess_type_str,"%d",mess_type+1);
					SetRspMessType(mess_type_str, send_buffer);
					ret = signoutRsp.SerializeToArray(send_buffer+3,10240);
                SocketSendRsp(fd,signoutRsp.ByteSize()+3,send_buffer);
				break;
			default:
				printf("unknown mess_type: %d\n",mess_type);
				break;
		}

    }
    
}


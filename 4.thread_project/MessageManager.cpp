#include "common/ret_value.h"
#include "MessageManager.h"

int MessageManager::Init(DbManager* db_svr){
    db_svr_ = db_svr;
	printf("MessageManager Init\n");
	return SUCCESS;
}

void MessageManager::Start(){
	int ret=db_svr_->GetMessBegin();
	int ui=0;
	if(ret==SUCCESS){
		ret=db_svr_->GetMessOneByOne(&messages_[ui]);
		if(ret==SUCCESS)ui++;
Id2Mess_map[messages_[ui].sublisher()]=ui;
		while(ret!=DB_NO_MORE_DATA){
			ret=db_svr_->GetMessOneByOne(&messages_[ui]);
			if(ret==SUCCESS)ui++;
Id2Mess_map[messages_[ui].sublisher()]=ui;
		}
		ret=db_svr_->GetMessEnd();
		set_message_count(ui);
	}else{
		printf("[Error   ]MessageManager Start fail\n");
		return;
	}
	printf("========================================\n");
	for(int i=0;i<message_count();i++){
		// printf("| %d | publisher:%s |  time:%d | message_id:%d |\n",photos_[i].user_id(),\
		// photos_[i].last_publisher(),photos_[i].last_publish_time(),photos_[i].last_publish_message_id());
		printf("| %d | publisher:%s |  time:%d | message_id:%d |\n",messages_[i].sublisher(),\
			messages_[i].publisher(), messages_[i].publish_time(),messages_[i].message_id());
		printf("recv : [%s]\n", messages_[i].content());
	}
	printf("========================================\n");
	printf("MessageManager::Start Over\n");
}

void MessageManager::Proc(){
	SavePhotos();
	// printf("MessageManager::Proc\n");
}

void MessageManager::Shutdown(){
	SavePhotos();
	// printf("MessageManager::Shutdown\n");
}

void MessageManager::Restart(){
	SavePhotos();
	Start();
	printf("MessageManager::Restart\n");
}

MessageInfo* MessageManager::GetMessageByUserId(int user_id){
	// for(int i=0;i<message_count();i++){
	// 	if(messages_[i].sublisher()==user_id){
	// 		return &messages_[i];
	// 	}
	// }
	int idx=Id2Mess_map[user_id]-1;
	if(idx>=0){
		return &messages_[idx];
	}
	return NULL;
}
MessageInfo* MessageManager::GetMessageByMessageId(int message_id){
	for(int i=0;i<message_count();i++){
		if(messages_[i].message_id()==message_id){
			return &messages_[i];
		}
	}
	return NULL;
}
int MessageManager::PublishMessage(MessageInfo message){
	messages_[message_count()].set_content(message.content());
	messages_[message_count()].set_message_id(message_count()+1);
	messages_[message_count()].set_publisher(message.publisher());
	messages_[message_count()].set_sublisher(message.sublisher());
	messages_[message_count()].set_publish_time(message.publish_time());
	messages_[message_count()].set_db_flag(FLAG_INSERT);
Id2Mess_map[messages_[message_count()].sublisher()]=message_count()+1;
	set_message_count(message_count()+1);
	return message_count();
}
int MessageManager::DeleteMessage(int message_id){
	for(int i=0;i < message_count();i++){
		if(messages_[i].message_id()==message_id){
			MessageInfo mess = messages_[message_count()-1];
			messages_[i].set_content(mess.content());
			// messages_[i]=messages_[message_count()-1];
			messages_[i].set_message_id(mess.message_id());
			messages_[i].set_publisher(mess.publisher());
			messages_[i].set_sublisher(mess.sublisher());
			messages_[i].set_publish_time(mess.publish_time());
			set_message_count(message_count()-1);
			return SUCCESS;
		}
	}
	
	return MESSAGE_NOT_EXIST;
}

int MessageManager::DeleteMessageByUserId(int user_id){
	int idx = Id2Mess_map[user_id]-1;
	if(idx>=0){
		MessageInfo mess = messages_[message_count()-1];
		messages_[idx].set_content(mess.content());
		// messages_[idx]=messages_[message_count()-1];
		messages_[idx].set_message_id(mess.message_id());
		messages_[idx].set_publisher(mess.publisher());
		messages_[idx].set_sublisher(mess.sublisher());
		messages_[idx].set_publish_time(mess.publish_time());
		set_message_count(message_count()-1);
		return SUCCESS;
	}
	return MESSAGE_NOT_EXIST;
}


void MessageManager::SavePhotos(){
	for(int i=0;i< message_count();i++){
		/* 1 delete */
		if(messages_[i].db_flag()==FLAG_DELETE){
			int user_id = messages_[i].sublisher();
			
			db_svr_->DeleteMess(user_id);
            messages_[i].set_db_flag(FLAG_INIT);
		}
		/* 2 update */
		if(messages_[i].db_flag()==FLAG_UPDATE){
			db_svr_->UpdateMess(&messages_[i]);
                        messages_[i].set_db_flag(FLAG_INIT);
		}
		/* 3 insert */
		if(messages_[i].db_flag()==FLAG_INSERT){
			if(db_svr_->InsertMess(&messages_[i]) == SUCCESS){
				printf("Insert Message %d \n",messages_[i].sublisher());
			}else{
				printf("Insert Message fail %d \n",messages_[i].sublisher());
				return;
			}
			messages_[i].set_db_flag(FLAG_INIT);
		}
	}
	return;
}
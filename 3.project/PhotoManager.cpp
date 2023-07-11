#include "PhotoManager.h"
#include "common/ret_value.h"
#include "common/mess_type.h"


PhotoInfo* PhotoManager::GetPhoto(int user_id){
	for(int i=0;i<photo_count_;i++){
		if(photos_[i].user_id()==user_id){
			return &photos_[i];
		}
	}
	return nullptr;
}

int PhotoManager::UpdatePhoto(int user_id,const char* publisher,int publish_time,int publish_message_id){
	PhotoInfo* photo=GetPhoto(user_id);
	if(photo==nullptr){
		photo = &photos_[photo_count()];
		set_photo_count(photo_count()+1);
		photo->set_db_flag(FLAG_INSERT);
		// return PHOTO_NOT_EXIST;
	}else{
		photo->set_db_flag(FLAG_UPDATE);
		if(publish_message_id == PHOTO_NOT_EXIST){
			photo->set_last_publish_message_id(publish_message_id);
			return SUCCESS;
		}
	}
	photo->set_user_id(user_id);//need not
	photo->set_last_publisher(publisher);
	photo->set_last_publish_time(publish_time);
	photo->set_last_publish_message_id(publish_message_id);
	return SUCCESS;
}

int PhotoManager::Init(DbManager* db_svr){
    db_svr_ = db_svr;
	printf("PhotoManager Init\n");
	return SUCCESS;
}

void PhotoManager::Start(){
	int ret=db_svr_->GetPhotosBegin();
	int ui=0;
	if(ret==SUCCESS){
		ret=db_svr_->GetPhotosOneByOne(&photos_[ui]);
		if(ret==SUCCESS)ui++;
		while(ret!=DB_NO_MORE_DATA){
			ret=db_svr_->GetPhotosOneByOne(&photos_[ui]);
			if(ret==SUCCESS)ui++;
		}
		ret=db_svr_->GetPhotosEnd();
		set_photo_count(ui);
	}else{
		printf("[Error   ]PhotoManager Start fail\n");
		return;
	}
	printf("========================================\n");
	for(int i=0;i<photo_count();i++){
		printf("| %d | last_publisher:%s |  last_publish_time:%d | message_id:%d |\n",photos_[i].user_id(),\
		photos_[i].last_publisher(),photos_[i].last_publish_time(),photos_[i].last_publish_message_id());
	}
	printf("========================================\n");
	printf("PhotoManager::Start Over\n");
}
void PhotoManager::Proc(){
	SavePhotos();
	printf("PhotoManager::Proc\n");
}
void PhotoManager::Shutdown(){
	SavePhotos();
	printf("PhotoManager::Shutdown\n");
}
void PhotoManager::Restart(){
	SavePhotos();
	Start();
	printf("PhotoManager::Restart\n");
}


void PhotoManager::SavePhotos(){
	for(int i=0;i< photo_count();i++){
		/* 1 delete */
		if(photos_[i].db_flag()==FLAG_DELETE){
			int user_id = photos_[i].user_id();
			
			db_svr_->DeletePhoto(user_id);
                        photos_[i].set_db_flag(FLAG_INIT);
		}
		/* 2 update */
		if(photos_[i].db_flag()==FLAG_UPDATE){
			db_svr_->UpdatePhoto(&photos_[i]);
                        photos_[i].set_db_flag(FLAG_INIT);
		}
		/* 3 insert */
		if(photos_[i].db_flag()==FLAG_INSERT){
			if(db_svr_->InsertPhoto(&photos_[i]) == SUCCESS){
				printf("Insert Photo %d \n",photos_[i].user_id());
			}else{
				printf("Insert Photo fail %d \n",photos_[i].user_id());
				return;
			}
			photos_[i].set_db_flag(FLAG_INIT);
		}
	}
	return;
}
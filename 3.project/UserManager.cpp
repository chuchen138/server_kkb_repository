#include <stdio.h>
#include "common/ret_value.h"
#include "UserManager.h"
#include "pb/user_info_base.pb.h"
// #include "mydb.h"

int UserManager::LoginCheck(const char* user_name, const char* password){
	for(int i=0;i<user_count_;i++){
		if(strcmp(users[i].user_name(),user_name)==0){
			if(strcmp(users[i].password(),password)==0){
				return SUCCESS;
			}else{
				return WRONG_PASSWORD;
			}
		}
	}
	return USER_NOT_EXIST;
}

int UserManager::GetUserIdByUserName(const char* user_name){
        for(int i=0;i<user_count_;i++){
                if(strcmp(users[i].user_name(),user_name)==0){
                	return users[i].user_id();
                }
        }
        return USER_NOT_EXIST;
}

int UserManager::Init(DbManager* db_svr){
    db_svr_ = db_svr;
	printf("UserManager Init\n");
	return SUCCESS;
}

int UserManager::Proc(){
	printf("UserManager Running\n");
    return SUCCESS;
}

int UserManager::Start(){
    int used_user_id=db_svr_->GetUserId();
	set_cur_user_id(used_user_id);
	int ret=db_svr_->GetUsersBegin();
	int ui=0;
	if(ret==SUCCESS){
		ret=db_svr_->GetUsersOneByOne(&users[ui]);
		if(ret==SUCCESS)ui++;
		while(ret!=DB_NO_MORE_DATA){
			ret=db_svr_->GetUsersOneByOne(&users[ui]);
			if(ret==SUCCESS)ui++;
		}
		ret=db_svr_->GetUsersEnd();
		set_user_count(ui);
	}else{
		printf("[Error   ]UserManager Start fail\n");
		return ret;
	}
	ShowAll();
	printf("UserManager Start\n");
	return SUCCESS;
}

int UserManager::Shutdown(){
    SaveUsers();
	printf("UserManager Shutdown\n");
    return SUCCESS;
}

int UserManager::Restart(){
	printf("UserManager Restart\n");
    return SUCCESS;
}

int UserManager::ShowAll(){
	printf("========================================\n");
	for(int i=0;i<user_count();i++){
		printf("| %d | %s | %s |\n",users[i].user_id(),users[i].user_name(),users[i].password());
	}
	printf("========================================\n");
	return 0;
}

UserInfo* UserManager::GetUser(int user_id) {
	for(int i=0;i<user_count_;i++) {
		if(users[i].user_id() == user_id) {
			return &users[i];
		}
	}
	return NULL;
}


int UserManager::CreateUser(const char* user_name,const char* pswd,int from,int time_now) {
	for(int i=0;i<user_count_;i++){
		if(strcmp(users[i].user_name(),user_name)==0){
			printf("USER EXIST %d %s\n",users[i].user_id(),users[i].user_name());
			return USER_EXIST;
		}
	}
	//int cur_user_id=1;//GetVarUserId();//todo get user_id from tb_var str='user_id';	

	if(user_count_<10239){
		users[user_count_].set_user_id(cur_user_id());
		users[user_count_].set_user_name(user_name);
		users[user_count_].set_password(pswd);
		users[user_count_].set_from(from);
		users[user_count_].set_reg_time(time_now);
		user_count_++;
	}
	set_cur_user_id(cur_user_id()+1);
	set_reg_num(reg_num()+1);

	return 0;
}

int UserManager::DeleteUser(int user_id){
	for(int i=0;i<user_count_;i++){
		if(users[i].user_id()==user_id){
			printf("USER EXIST %d %s\n",users[i].user_id(),users[i].user_name());
            users[i].set_db_flag(FLAG_DELETE);
			// users[i].set_user_id(-1);
			return USER_EXIST;
		}
	}
	return 0;
}

int UserManager::UpdateUserLoginTime(int user_id,int time_now){
	UserInfo* user=GetUser(user_id);
	if(user==NULL){
		printf("USER_NOT_EXIST %d\n",user_id);
		return USER_NOT_EXIST;
	}
	user->set_login_time(time_now);
	return SUCCESS;
}
int UserManager::UpdateUserFreshTime(int user_id,int time_now){
	UserInfo* user=GetUser(user_id);
        if(user==NULL){
                printf("USER_NOT_EXIST %d\n",user_id);
                return USER_NOT_EXIST;
        }
        user->set_fresh_time(time_now);
        return SUCCESS;
}
int UserManager::UserLogout(int user_id,int now){
	UserInfo* user=GetUser(user_id);
	if(user==NULL){
		return USER_NOT_EXIST;
	}
	user->set_logout_time(now);
	return SUCCESS;
}

int UserManager::CheckExist(int user_id){
	UserInfo* user=GetUser(user_id);
        if(user==NULL){
                printf("USER_NOT_EXIST %d\n",user_id);
                return USER_NOT_EXIST;
        }
	return USER_EXIST;
}

int UserManager::SaveUsers(){
	for(int i=0;i<user_count_;i++){
		/* 1 delete */
		if(users[i].db_flag()==FLAG_DELETE){
			db_svr_->DeleteUser(&users[i]);
                        users[i].set_db_flag(FLAG_INIT);
		}
		/* 2 update */
		if(users[i].db_flag()==FLAG_UPDATE){
			db_svr_->UpdateUser(&users[i]);
                        users[i].set_db_flag(FLAG_INIT);
		}
		/* 3 insert */
		if(users[i].db_flag()==FLAG_INSERT){
			db_svr_->InsertUser(&users[i]);
			users[i].set_db_flag(FLAG_INIT);
		}
	}
	return 0;
}

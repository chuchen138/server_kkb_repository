#include <stdio.h>
#include "common/ret_value.h"
#include "UserManager.h"

void UserManager::Init(){
	printf("UserManager Init\n");
}

void UserManager::Proc(){
	printf("UserManager Running\n");
}

void UserManager::Start(){
	Init();
	printf("UserManager Start\n");
}

void UserManager::Shutdown(){
	printf("UserManager Shutdown\n");
}

void UserManager::Restart(){
	printf("UserManager Restart\n");
}

int UserManager::ShowAll(){
	printf("========================================\n");
	for(int i=0;i<user_count_;i++){
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

int UserManager::CreateUser(int user_id, char* user_name, char* pswd) {
	for(int i=0;i<user_count_;i++){
		if(users[i].user_id()==user_id){
			printf("USER EXIST %d %s\n",users[i].user_id(),users[i].user_name());
			return USER_EXIST;
		}
	}
	if(user_count_<10239){
		users[user_count_].set_user_id(user_id);
		users[user_count_].set_user_name(user_name);
		users[user_count_].set_password(pswd);
		user_count_++;
	}

	return 0;
}

int UserManager::DeleteUser(int user_id){
	for(int i=0;i<user_count_;i++){
		if(users[i].user_id()==user_id){
			printf("USER EXIST %d %s\n",users[i].user_id(),users[i].user_name());
			users[i].set_user_id(-1);
			return USER_EXIST;
		}
	}

	return 0;
}

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

int UserManager::Init(){
    set_user_count(0);
    conn = mysql_init(NULL);
	if(conn == NULL){
		return DB_CONN_INIT_FAIL;
	}
	conn = mysql_real_connect(conn,"127.0.0.1","root","123","sxg",0,NULL,0);
	if(conn == NULL){
		return DB_CONN_CONNECT_FAIL;
	}
	if(mysql_query(conn,"select * from tb_user;")){
		printf("query fail : %s \n",mysql_error(conn));
		return DB_QUERY_FAIL;
	}else{
		result = mysql_use_result(conn);
		if(result){
			int num_fields_1 = mysql_num_fields(result);
			int num_fields_2 = mysql_field_count(conn);
			//printf("num_fields_1 %d, num_fields_2 %d\n",num_fields_1,num_fields_2);
			for(int i=0;i<num_fields_2;i++){
				row = mysql_fetch_row(result);
				if(row == NULL) {
					break;
				}
				for(int j=0;j<num_fields_1;j++){
					printf("%s\t",row[j]);
				}
				// 0 - id
				// 1 - SerializeString

				//users[i].set_user_id(str2int(row[0]));
				//users[i].set_user_name();
				sxg::UserInfoBase pb_user;
				pb_user.ParseFromArray(row[1],sizeof(row[1]));
				//pb_user.user_id();
				users[i].set_user_id(pb_user.user_id());
				//users[i].set_user_name(pb_user.user_name());
				//users[i].set_nick_name(pb_user.nick_name());
				users[i].set_db_flag(FLAG_INIT);
				printf("\n");

			}
			set_user_count(num_fields_2);
		}
		mysql_free_result(result);
	}	
	printf("UserManager Init\n");
	return SUCCESS;
}

int UserManager::Proc(){
	printf("UserManager Running\n");
    return SUCCESS;
}

int UserManager::Start(){
    set_cur_user_id(10001);
	int ret=Init();
	if(ret!=SUCCESS){
		printf("UserManager Start Fail %d\n",ret);
		return ret;
	}
	printf("UserManager Start\n");
	return SUCCESS;
}

int UserManager::Shutdown(){
    for(int i=0;i<user_count_;i++){
		//1 delete
		if(users[i].db_flag()==FLAG_DELETE){
			char user_id[256];
			sprintf(user_id,"%d",users[i].user_id());
			string deleSql="delete from tb_user where user_id=";
			deleSql+=user_id;
			deleSql+=";";
			//printf("del:%s\n",deleSql.c_str());
			if(mysql_query(conn,deleSql.c_str())){
         			printf("delete fail : %s \n",mysql_error(conn));
                		return DB_QUERY_FAIL;
        		}
		}
		//2 update
		if(users[i].db_flag()==FLAG_UPDATE||1){
			sxg::UserInfoBase pb_user;
			pb_user.set_user_id(users[i].user_id());
			//pb_user.set_user_name(users[i].user_name());
			
			pb_user.set_login_time(users[i].login_time());
			char data[10240];
			pb_user.SerializeToArray(data,10240);
			char user_id[256];
                        sprintf(user_id,"%d",users[i].user_id());
			string updateSql="update tb_user set user_info='";
			updateSql+=data;
			updateSql+="' where user_id=";
			updateSql+=user_id;
			updateSql+=";";
			printf("upSql:%s\n",updateSql.c_str());
			//+data+"' where id="+pb_user.user_id()+";";
			if(mysql_query(conn,updateSql.c_str())){
                		printf("update fail : %s \n",mysql_error(conn));
                		return DB_QUERY_FAIL;
        	}
		}
	}
	printf("UserManager Shutdown\n");
    return SUCCESS;
}

int UserManager::Restart(){
	printf("UserManager Restart\n");
    return SUCCESS;
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

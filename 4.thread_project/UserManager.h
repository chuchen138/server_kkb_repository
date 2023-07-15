#ifndef _USERMANAGER_H
#define _USERMANAGER_H
#include "UserInfo.h"
#include "mydb.h"

class UserManager{
private:
    UserInfo users[10240];
    GETSETVAR(int,user_count)
	GETSETVAR(int,reg_num)
	GETSETVAR(int,cur_user_id)
    DbManager* db_svr_;
    std::unordered_map<int,int> user_id_idx;
    std::unordered_map<string,int> name_idx;

public:
    int Init(DbManager *db_svr);
    int Start();
    int Proc();
    int Shutdown();
    int Restart();
public:
    int ShowAll();
    UserInfo* GetUser(int user_id);
    int CheckExist(int user_id);
	int CreateUser(const char* user_name,const char *pswd, int from, int time_now);
	int DeleteUser(int user_id);
    int SaveUsers();
	int LoginCheck(const char* user_name,const char* user_pswd);
	int UserLogout(int user_id,int now);
	int GetUserIdByUserName(const char* user_name);
	const char* GetUserNameByUserId(const int user_id);
	int UpdateUserLoginTime(int user_id, int time_now);
	int UpdateUserFreshTime(int user_id, int time_now);
};


#endif

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
    int Init();

private:
    MYSQL *conn;
    MYSQL_RES *result;
    MYSQL_ROW row;

public:
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
	int LoginCheck(const char* user_name,const char* user_pswd);
	int UserLogout(int user_id,int now);
	int GetUserIdByUserName(const char* user_name);
	int UpdateUserLoginTime(int user_id, int time_now);
	int UpdateUserFreshTime(int user_id, int time_now);
};


#endif

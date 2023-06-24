#ifndef _MYDB_H
#define _MYDB_H
#include <iostream>
#include "mysql/mysql.h"
#include "common/ret_value.h"
#include "pb/user_info_base.pb.h"
#include "UserInfo.h"

using namespace std;
class DbManager{
private:
    GETSETVAR(int,transection)
    MYSQL *conne;
    MYSQL_RES *result;
    MYSQL_ROW row;

public:
    int Init();
    int initDb(string host, string user, string passwd, string db_name);
    int execSql(string sql);
    //int GetUserInfo(UserInfo &users);
    int GetUserId();
	int GetUsersBegin();
	int GetUsersOneByOne(UserInfo* user);
	int GetUsersEnd();
	int InsertUser(UserInfo* user);
	int UpdateUser(UserInfo* user);
	int DeleteUser(UserInfo* user);
};


#endif

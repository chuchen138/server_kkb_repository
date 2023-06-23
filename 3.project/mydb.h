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
    MYSQL *conne;
    MYSQL_RES *result;
    MYSQL_ROW row;

public:
    int initDb(string host, string user, string passwd, string db_name);
    int execSql(string sql);
    //int GetUserInfo(UserInfo &users);
};


#endif

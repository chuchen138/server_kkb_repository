#ifndef _MYDB_H
#define _MYDB_H
#include <iostream>
#include "mysql/mysql.h"
#include "common/ret_value.h"
#include "pb/user_info_base.pb.h"
#include "pb/message_info_base.pb.h"
#include "pb/photo_info_base.pb.h"
#include "pb/relation_info_base.pb.h"
#include "UserInfo.h"
#include "RelationInfo.h"
#include "PhotoInfo.h"
#include "MessageInfo.h"

using namespace std;
class DbManager{
private:
    GETSETVAR(int,transection) // 判断数据库是否繁忙
    GETSETVAR(int,user_num_count) 
    MYSQL *conne;
    MYSQL_RES *result;
    MYSQL_ROW row;
    // MYSQL_RES *resultRela;
    // MYSQL_ROW rowRela;
    int GetBegin(string str);
	// int GetUsersOneByOne(UserInfo* user);
	int GetEnd();
	// int InsertSql();
	// int UpdateSql();
	// int DeleteSql();

public:
    int Init();
    int initDb(string host, string user, string passwd, string db_name);
    int execSql(string sql);
    //int GetUserInfo(UserInfo &users);
    int GetUserId();
	int DeleteMessAndPhoto(int user_id);

	int GetUsersBegin();
	int GetUsersOneByOne(UserInfo* user);
	int GetUsersEnd();
	int InsertUser(UserInfo* user);
	int UpdateUser(UserInfo* user);
	int DeleteUser(UserInfo* user);

	int GetRelasBegin();
	int GetRelasOneByOne(RelationInfo* rela);
	int GetRelasEnd();
	int InsertRela(RelationInfo* rela);
	int UpdateRela(RelationInfo* rela);
	int DeleteRela(int user_id);

	int GetMessBegin();
	int GetMessOneByOne(MessageInfo* rela);
	int GetMessEnd();
	int InsertMess(MessageInfo* rela);
	int UpdateMess(MessageInfo* rela);
	int DeleteMess(int user_id);

	int GetPhotosBegin();
	int GetPhotosOneByOne(PhotoInfo* rela);
	int GetPhotosEnd();
	int InsertPhoto(PhotoInfo* rela);
	int UpdatePhoto(PhotoInfo* rela);
	int DeletePhoto(int user_id);
};


#endif

/*************************************************************************
	> File Name: mydb.cpp
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Sat 18 Mar 2023 06:14:19 PM CST
 ************************************************************************/

#include<iostream>
#include<cstdio>
#include<cstring>
#include "mydb.h"
using namespace std;

char mysql_username[256];
char mysql_password[256];

int ReadMysqlPassword(){
	FILE* f=fopen("mysql.ini","r");
    if(f == NULL){
        return -1;
    }
	fscanf(f,"%s",mysql_username);
	fscanf(f,"%s",mysql_password);
	/*printf("mysql_username:%s,mysql_password:%s\n",mysql_username,mysql_password);*/
	fclose(f);
    return 0;
}

int DbManager::Init(){
	int ret=initDb("127.0.0.1","root","123","sxg");
	set_transection(0);
	result=NULL;
	printf("[INFO    ]DbManager Init:%d\n",ret);
	return ret;
}

int DbManager::initDb(string host,string user, string passwd, string db_name){
    int ret = ReadMysqlPassword();
    conne = mysql_init(NULL);
    if(conne == NULL) return DB_CONN_INIT_FAIL;
    if(ret < 0)
        conne = mysql_real_connect(conne, host.c_str(), user.c_str(),passwd.c_str(),db_name.c_str(),0 ,NULL,0);
    else
        conne = mysql_real_connect(conne,host.c_str(),mysql_username,mysql_password,db_name.c_str(),0,NULL,0);
    if(conne == NULL) return DB_CONN_CONNECT_FAIL;
    return SUCCESS;
}


int DbManager::execSql(string sql){
    if(mysql_query(conne, sql.c_str())){
        cout << "sql failed!\n" ;
        return DB_QUERY_FAIL;
    }else{
        result = mysql_use_result(conne);
        if(result){
            int num_fields_1 = mysql_num_fields(result);
            int num_fields_2 = mysql_field_count(conne);
            int row_count = mysql_num_rows(result);
            printf("row_count %d, num_fields_1 %d, num_fields_2 %d\n",row_count,num_fields_1,num_fields_2);
			while(result){
				row = mysql_fetch_row(result);
				if(row == NULL) {
					printf("empty row\n");
					break;
				}
				for(int j=0;j<num_fields_1;j++){
					printf("%s\t",row[j]);
				}
				printf("\n");

			}
			row_count = mysql_num_rows(result);
            printf("row_count %d, num_fields_1 %d, num_fields_2 %d\n",row_count,num_fields_1,num_fields_2);
        }
        mysql_free_result(result);
    }


    return SUCCESS;
}

int DbManager::GetUsersBegin(){
	if(transection()==1){
		return DB_IS_BUSY;
	}
	set_transection(1);
	int ret=mysql_query(conne,"select * from tb_user;");
	if(ret){
		printf("[WARN    ]query fail : %d %s \n",ret,mysql_error(conne));
		set_transection(0);
		return DB_QUERY_FAIL;
	}
	result = mysql_use_result(conne);
	return SUCCESS;
}

int DbManager::GetUsersOneByOne(UserInfo* user){
	if(result){
		row = mysql_fetch_row(result);
		if(row==NULL){
			printf("[INFO    ]no more user\n");
			return DB_NO_MORE_DATA;
		}
		sxg::UserInfoBase pb_user;
		pb_user.ParseFromArray(row[1],10240);
		printf("[INFO    ]user_id:%d,user_name:%s\n",pb_user.user_id(),pb_user.user_name().c_str());
		user->set_user_id(pb_user.user_id());
		user->set_user_name(pb_user.user_name().c_str());
		user->set_password(pb_user.password().c_str());
	}else{
		return DB_NO_MORE_DATA;
	}
	return SUCCESS;
}

int DbManager::GetUsersEnd(){
	mysql_free_result(result);
	set_transection(0);
	return SUCCESS;
}
int DbManager::UpdateUser(UserInfo* user){
	return SUCCESS;
}
int DbManager::DeleteUser(UserInfo* user){
	return SUCCESS;
}
int DbManager::InsertUser(UserInfo* user){
	sxg::UserInfoBase pb_user;
	pb_user.set_user_id(user->user_id());
	pb_user.set_user_name(user->user_name());
	pb_user.set_password(user->password());
	char data[10240];
	pb_user.SerializeToArray(data,pb_user.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",user->user_id());
	string insertSql="insert into tb_user values(";
	insertSql+=user_id;
	insertSql+=",'";
	insertSql+=data;
	insertSql+="');";
	printf("[DEBUG   ]insertSql:%s\n",insertSql.c_str());
	int ret=mysql_query(conne,insertSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[WARN    ]insert fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}

int DbManager::GetUserId(){
	int used_user_id=10000;
	int ret=mysql_query(conne,"select value from tb_var where key_str='user_id';");
	if(ret){
		printf("[WARN    ]query user_id fail : %d %s \n",ret,mysql_error(conne));
		return used_user_id;
	}else{
		result = mysql_use_result(conne);
		if(result){
			row = mysql_fetch_row(result);
			if(row == NULL) {
				printf("[DEBUG   ]query user_id empty\n");
			}
			sscanf(row[0],"%d",&used_user_id);
		}
		mysql_free_result(result);
	}
	return used_user_id;
}



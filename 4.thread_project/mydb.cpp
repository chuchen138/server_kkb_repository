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
			set_user_num_count(row_count);
        }
        mysql_free_result(result);
    }


    return SUCCESS;
}

int DbManager::DeleteMessAndPhoto(int user_id){
	string deletetSql="delete from tb_message where id=";
	char user_id_char[10];
	sprintf(user_id_char,"%d",user_id);
	deletetSql += user_id_char;
	printf("[DEBUG   ]deleteSql:%s\n",deletetSql.c_str());
	int ret=mysql_query(conne,deletetSql.c_str());
	if(ret != 0){
		printf("[WARN    ]delete fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	string deletetSql2="delete from tb_photo where id=";
	deletetSql2 += user_id_char;
	printf("[DEBUG   ]deleteSql:%s\n",deletetSql2.c_str());
	ret=mysql_query(conne,deletetSql2.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[WARN    ]delete fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}

int DbManager::GetBegin(string str){
	if(transection()==1){
		return DB_IS_BUSY;
	}
	set_transection(1);
	char buff[128] = {0};

	sprintf(buff, "select * from %s;",str.c_str());
	int ret=mysql_query(conne, buff);
	if(ret){
		printf("[WARN    ]query fail : %d %s \n",ret,mysql_error(conne));
		set_transection(0);
		return DB_QUERY_FAIL;
	}
	result = mysql_use_result(conne);
	return SUCCESS;
}
int DbManager::GetEnd(){
	mysql_free_result(result);
	set_transection(0);
	return SUCCESS;
}
int DbManager::GetUsersBegin(){
	return GetBegin("tb_user");
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
	return GetEnd();
}


int DbManager::GetRelasBegin(){
	return GetBegin("tb_relation");
}
int DbManager::GetRelasOneByOne(RelationInfo* rela){
	if(result){
		row = mysql_fetch_row(result);
		if(row==NULL){
			printf("[INFO    ]no more rela\n");
			return DB_NO_MORE_DATA;
		}
		sxg::RelationInfoBase pb_rela;
		pb_rela.ParseFromArray(row[1],10240);
		printf("[INFO    ]user_id:%d | friend: ",pb_rela.user_id());
		rela->set_user_id(pb_rela.user_id());
		for(int i = 0; i < pb_rela.friend_user_id_size(); i++){
			rela->AddFriend(pb_rela.friend_user_id(i));
			printf("%d ",pb_rela.friend_user_id(i));
		}
		printf("| black: ");
		rela->set_friend_count(pb_rela.friend_user_id_size());
		for(int i = 0; i < pb_rela.black_user_id_size(); i++){
			rela->AddBlack(pb_rela.black_user_id(i));
			printf("%d ",pb_rela.black_user_id(i));
		}
		printf("\n");
		rela->set_black_count(pb_rela.black_user_id_size());
	}else{
		return DB_NO_MORE_DATA;
	}
	return SUCCESS;
}
int DbManager::GetRelasEnd(){
	return GetEnd();
}

int DbManager::UpdateUser(UserInfo* user){
	sxg::UserInfoBase pb_user;
	pb_user.set_user_id(user->user_id());
	pb_user.set_user_name(user->user_name());
	pb_user.set_password(user->password());
	char data[10240];
	pb_user.SerializeToArray(data,pb_user.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",user->user_id());
	string updateSql="UPDATE tb_user SET info='";
	updateSql+=data;
	updateSql+=",' WHERE id=";
	updateSql+=user_id;
	updateSql+=";";
	printf("[DEBUG   ]updateSql:%s\n",updateSql.c_str());
	int ret=mysql_query(conne,updateSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[WARN    ]update fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::DeleteUser(UserInfo* user){
	string deletetSql="delete from tb_user where id=";
	char user_id[256];
	sprintf(user_id,"%d",user->user_id());
	deletetSql += user_id;
	printf("[DEBUG   ]deleteSql:%s\n",deletetSql.c_str());
	int ret=mysql_query(conne,deletetSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[WARN    ]delete fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::InsertUser(UserInfo* user){
	sxg::UserInfoBase pb_user;
	pb_user.set_user_id(user->user_id());
	pb_user.set_user_name(user->user_name());
	pb_user.set_password(user->password());
	char data[10240]={0};
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

int DbManager::InsertRela(RelationInfo* rela){
	sxg::RelationInfoBase pb_rela;
	pb_rela.set_user_id(rela->user_id());
	for(int i = 0; i < rela->friend_count(); i++){
		pb_rela.add_friend_user_id(rela->friend_list_[i]);
		printf("[DbManager::DEBUG   ]friend_list_id:%d\n",rela->friend_list_[i]);
	}
	for(int i = 0; i < rela->black_count(); i++){
		pb_rela.add_black_user_id(rela->black_list_[i]);
		printf("[DbManager::DEBUG   ]black_list_id:%d\n",rela->black_list_[i]);
	}
	char data[10240]={0};
	pb_rela.SerializeToArray(data,pb_rela.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",rela->user_id());
	string insertSql="insert into tb_relation values(";
	insertSql+=user_id;
	insertSql+=",'";
	insertSql+=data;
	insertSql+="');";
	printf("[DbManager::DEBUG   ]insertSql:%s\n",insertSql.c_str());
	int ret=mysql_query(conne,insertSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[WARN    ]insert fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::UpdateRela(RelationInfo* rela){
	sxg::RelationInfoBase pb_rela;
	pb_rela.set_user_id(rela->user_id());
	for(int i = 0; i < rela->friend_count(); i++){
		pb_rela.add_friend_user_id(rela->friend_list_[i]);
		printf("[DbManager::DEBUG   ]friend_list_id:%d\n",rela->friend_list_[i]);
	}
	for(int i = 0; i < rela->black_count(); i++){
		pb_rela.add_black_user_id(rela->black_list_[i]);
		printf("[DbManager::DEBUG   ]black_list_id:%d\n",rela->black_list_[i]);
	}
	char data[10240];
	pb_rela.SerializeToArray(data,pb_rela.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",rela->user_id());
	string updateSql="UPDATE tb_relation SET info='";
	updateSql+=data;
	updateSql+=",' WHERE id=";
	updateSql+=user_id;
	updateSql+=";";
	printf("[DbManager::DEBUG   ]updateSql:%s\n",updateSql.c_str());
	int ret=mysql_query(conne,updateSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[DbManager::WARN    ]update fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::DeleteRela(int user_id){
	string deletetSql="delete from tb_relation where id=";
	char user_id_str[256];
	sprintf(user_id_str,"%d", user_id);
	deletetSql += user_id_str;
	printf("[DbManager::DEBUG   ]deleteSql:%s\n",deletetSql.c_str());
	int ret=mysql_query(conne,deletetSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[DbManager::WARN    ]delete fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}

int DbManager::GetMessBegin(){
	return GetBegin("tb_message");
}
int DbManager::GetMessOneByOne(MessageInfo* messinfo){
	if(result){
		row = mysql_fetch_row(result);
		if(row==NULL){
			printf("[INFO    ]no more photo\n");
			return DB_NO_MORE_DATA;
		}
		sxg::MessageInfoBase pb_mess;
		pb_mess.ParseFromArray(row[1],10240);
		printf("[INFO    ]user_id:%d | friend: ",pb_mess.sublisher());
		messinfo->set_sublisher(pb_mess.sublisher());
		messinfo->set_publisher(pb_mess.publisher().c_str());

		messinfo->set_message_id(pb_mess.message_id());
		messinfo->set_publish_time(pb_mess.publish_time());
		messinfo->set_content(pb_mess.content().c_str());
	}else{
		return DB_NO_MORE_DATA;
	}
	return SUCCESS;
}

int DbManager::GetMessEnd(){
	return GetEnd();
}
int DbManager::InsertMess(MessageInfo* messinfo){
	sxg::MessageInfoBase pb_mess;
	pb_mess.set_sublisher(messinfo->sublisher());
	pb_mess.set_publisher(messinfo->publisher());
	pb_mess.set_message_id(messinfo->message_id());
	pb_mess.set_publish_time(messinfo->publish_time());
	pb_mess.set_content(messinfo->content());

	char data[1024]={0};
	pb_mess.SerializeToArray(data,pb_mess.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",messinfo->sublisher());
	string insertSql="insert into tb_message values(";
	insertSql+=user_id;
	insertSql+=",'";
	insertSql+=data;
	insertSql+="');";
	printf("[DbManager::DEBUG   ]insertSql:%s\n",insertSql.c_str());
	int ret=mysql_query(conne,insertSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[WARN    ]insert fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::UpdateMess(MessageInfo* messinfo){
	sxg::MessageInfoBase pb_mess;
	pb_mess.set_sublisher(messinfo->sublisher());
	pb_mess.set_publisher(messinfo->publisher());
	pb_mess.set_message_id(messinfo->message_id());
	pb_mess.set_publish_time(messinfo->publish_time());
	pb_mess.set_content(messinfo->content());

	char data[10240];
	pb_mess.SerializeToArray(data,pb_mess.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",messinfo->sublisher());
	string updateSql="UPDATE tb_message SET info='";
	updateSql+=data;
	updateSql+=",' WHERE id=";
	updateSql+=user_id;
	updateSql+=";";
	printf("[DbManager::DEBUG   ]updateSql:%s\n",updateSql.c_str());
	int ret=mysql_query(conne,updateSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[DbManager::WARN    ]update fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::DeleteMess(int user_id){
	string deletetSql="delete from tb_message where id=";
	char user_id_str[256];
	sprintf(user_id_str,"%d", user_id);
	deletetSql += user_id_str;
	printf("[DbManager::DEBUG   ]deleteSql:%s\n",deletetSql.c_str());
	int ret=mysql_query(conne,deletetSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[DbManager::WARN    ]delete fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}


int DbManager::GetPhotosBegin(){
	return GetBegin("tb_photo");
}
int DbManager::GetPhotosOneByOne(PhotoInfo* photo){
	if(result){
		row = mysql_fetch_row(result);
		if(row==NULL){
			printf("[INFO    ]no more photo\n");
			return DB_NO_MORE_DATA;
		}
		sxg::PhotoInfoBase pb_photo;
		pb_photo.ParseFromArray(row[1],10240);
		printf("[INFO    ]user_id:%d | friend: ",pb_photo.user_id());
		photo->set_user_id(pb_photo.user_id());
		photo->set_last_publisher(pb_photo.last_publisher().c_str());
		photo->set_last_publish_time(pb_photo.last_publish_time());
		photo->set_last_publish_message_id(pb_photo.last_publish_message_id());
	}else{
		return DB_NO_MORE_DATA;
	}
	return SUCCESS;
}

int DbManager::GetPhotosEnd(){
	return GetEnd();
}
int DbManager::InsertPhoto(PhotoInfo* photo){
	sxg::PhotoInfoBase pb_photo;
	pb_photo.set_user_id(photo->user_id());
	pb_photo.set_last_publisher(photo->last_publisher());
	pb_photo.set_last_publish_time(photo->last_publish_time());
	pb_photo.set_last_publish_message_id(photo->last_publish_message_id());

	char data[1024]={0};
	pb_photo.SerializeToArray(data,pb_photo.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",photo->user_id());
	string insertSql="insert into tb_photo values(";
	insertSql+=user_id;
	insertSql+=",'";
	insertSql+=data;
	insertSql+="');";
	printf("[DbManager::DEBUG   ]insertSql:%s\n",insertSql.c_str());
	int ret=mysql_query(conne,insertSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[WARN    ]insert fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::UpdatePhoto(PhotoInfo* photo){
	sxg::PhotoInfoBase pb_photo;
	pb_photo.set_user_id(photo->user_id());
	pb_photo.set_last_publisher(photo->last_publisher());
	pb_photo.set_last_publish_time(photo->last_publish_time());
	pb_photo.set_last_publish_message_id(photo->last_publish_message_id());

	char data[10240];
	pb_photo.SerializeToArray(data,pb_photo.ByteSize());
	char user_id[256];
	sprintf(user_id,"%d",photo->user_id());
	string updateSql="UPDATE tb_photo SET info='";
	updateSql+=data;
	updateSql+=",' WHERE id=";
	updateSql+=user_id;
	updateSql+=";";
	printf("[DbManager::DEBUG   ]updateSql:%s\n",updateSql.c_str());
	int ret=mysql_query(conne,updateSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[DbManager::WARN    ]update fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}
int DbManager::DeletePhoto(int user_id){
	string deletetSql="delete from tb_photo where id=";
	char user_id_str[256];
	sprintf(user_id_str,"%d", user_id);
	deletetSql += user_id_str;
	printf("[DbManager::DEBUG   ]deleteSql:%s\n",deletetSql.c_str());
	int ret=mysql_query(conne,deletetSql.c_str());
	if(ret==0){
		return SUCCESS;
	}else{
		printf("[DbManager::WARN    ]delete fail : %d %s \n",ret,mysql_error(conne));
		return DB_QUERY_FAIL;
	}
	return SUCCESS;
}

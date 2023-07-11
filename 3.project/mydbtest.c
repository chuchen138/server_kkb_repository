/*************************************************************************
	> File Name: main.cpp
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Sat 18 Mar 2023 06:47:26 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
#include <iostream>
#include "mysql/mysql.h"
// #include "mydb.h"

void test(){
    MYSQL *conne;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int ret=mysql_query(conne,"select * from tb_relation;");
	if(ret){
		printf("[WARN    ]query fail : %d %s \n",ret,mysql_error(conne));
        return ;
	}
}

int main(){
    // DbManager DB;
    // DB.Init();
    
    // DB.execSql("select * from tb_relation;");
    // cout << "\n=====================\n";
    // DB.GetRelasBegin();
    // cout << "\n=====================\n";
    // DB.GetRelasEnd();
    // DB.execSql("insert into dbtest values(11, 'EFC',134,'absfhsdahf');");
    // DB.execSql("select * from dbtest;");
    // DB.execSql("update dbtest set num=555 where id=2;");
    // DB.execSql("select * from dbtest;");
    // DB.execSql("delete from dbtest where id=11;");


    return 0;
}

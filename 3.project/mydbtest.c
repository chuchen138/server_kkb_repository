/*************************************************************************
	> File Name: main.cpp
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Sat 18 Mar 2023 06:47:26 PM CST
 ************************************************************************/

#include<iostream>
using namespace std;
#include "mydb.h"

int main(){
    DbManager DB;
    DB.initDb("127.0.0.1","root","123","sxg");
    
    DB.execSql("select * from dbtest;");
    DB.execSql("insert into dbtest values(11, 'EFC',134,'absfhsdahf');");
    DB.execSql("select * from dbtest;");
    DB.execSql("update dbtest set num=555 where id=2;");
    DB.execSql("select * from dbtest;");
    DB.execSql("delete from dbtest where id=11;");


    return 0;
}

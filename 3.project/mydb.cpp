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


int DbManager::initDb(string host,string user, string passwd, string db_name){
    conne = mysql_init(NULL);
    if(conne == NULL) return DB_CONN_INIT_FAIL;

    conne = mysql_real_connect(conne, host.c_str(), user.c_str(),passwd.c_str(),db_name.c_str(),0 ,NULL,0);
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
            /*
            cout << "field1 = " << num_fields_1 << ", field2 = " << num_fields_2 << endl;
            for(int i = 0; i < num_fields_2; ++i){
                row = mysql_fetch_row(result);
                if(row == NULL) break;
                for(int j = 0; j < num_fields_1; ++j){
                    printf("%s\t", row[j]);
                }
                cout << "\n";
            }
            */
        }
        mysql_free_result(result);
    }


    return SUCCESS;
}

/*************************************************************************
	> File Name: user_info.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Thu 16 Mar 2023 05:00:02 PM CST
 ************************************************************************/

#ifndef _USER_INFO_H
#define _USER_INFO_H
#include "err_define.h"

typedef struct _UserInfo_{
    int user_id;
    char user_name[256];
    char nick_name[256];
    int reg_time; // 注册时间
    int from; // 渠道：微信，QQ
    int login_time;
    int last_time;
    int last_login_time;
    int fresh_time;
    // 密码
}UserInfo;
int user_create(UserInfo *user,char *user_name, int now, int from);
int user_login(UserInfo *user, int now);
int user_logout(UserInfo *user, int now);
int user_delete(UserInfo *user);
#endif

/*************************************************************************
	> File Name: friend_info.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Thu 16 Mar 2023 05:14:43 PM CST
 ************************************************************************/

#ifndef _FRIEND_INFO_H
#define _FRIEND_INFO_H
#include "err_define.h"
typedef struct _FriendInfo_{
    int user_id_1;
    int user_id_2;
    int type; // 0 normal, 1-blcocklist
}FriendInfo;

typedef struct _UserBrief_{
    int user_id;
    char user_name[256];
}UserBrief;

typedef struct _FriendList_{
    int user_id;
    struct UserBrief *FriendList;
    struct UserBrief *BlackList;
}FriendList;
int add_friend(UserInfo *self,UserInfo* other);
int delete_friend(UserInfo *self,UserInfo* other);
int add_black(UserInfo *self,UserInfo* other);
int delete_black(UserInfo *self, UserInfo* other);
#endif

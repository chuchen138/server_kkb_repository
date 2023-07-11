#ifndef _USERINFO_H
#define _USERINFO_H

#include <string.h>
#include "./common/my_micro.h"
// #include "./pb/user_info_base.pb.h"

// using namespace sxg;
struct UserInfo{
public:
    // void FromPb(sxg::UserInfoBase& user);
    // void ToPb(sxg::UserInfoBase& user);
    GETSETVAR(int,user_id)
    GETSETSTR(256,user_name)
    GETSETSTR(256,nick_name)
    GETSETVAR(int,reg_time)
    GETSETVAR(int,from)
    GETSETVAR(int,login_time)
    GETSETVAR(int,last_login_time)
    GETSETVAR(int,fresh_time)
    GETSETSTR(256,password)
	GETSETVAR(int,db_flag)
	GETSETVAR(int,logout_time)
	GETSETVAR(int,is_online)
};


#endif

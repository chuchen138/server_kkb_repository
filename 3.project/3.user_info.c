#include "user_info.h"
int user_create(UserInfo *user,char *user_name, int now, int from){

    user->user_id = 0;
    
    strncpy(user->user_name, user_name, 256);
    
    strcpy(user->nick_name, user->user_name);

    user->reg_time = now;

    user->from = from;

    user->login_time = 0;
    user->last_login_time = 0;
    user->fresh_time = 0;

    return 0;
}

int user_login(UserInfo *user, int now){
    int ret = getUser(user, user->user_id);
    if(ret == 0){
        user->login_time = now;
    }
    return ret;
    
}


int user_logout(UserInfo *user, int now){
    int ret = Logout(user);
    if(ret == 0){
        // DB save
    }
    return ret;
}


int user_delete(UserInfo *user){
    int ret = deleteUser(user);
    if(ret == 0){
        // DB save
    }
    return ret;
}

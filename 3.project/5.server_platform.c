#include"err_define.h"

#define MAX_USER 1024

UserInfo *_UserFromDB_[MAX_USER];
FriendInfo *_FriendFromDB_;

int init(){
    // get from DB
    // (user->id)

    return 0;
}

int shutdown(){
    int user_size = sizeof(_UserFromDB_)/sizeof(UserInfo); 
    for(int i = 0; i < user_size; ++i){
        if(_UserFromDB_[i]->user_id != USER_NEED_DELETE){
            // WRITE DB
        }
    }
    return 0;
}

/**
 * return user idx else return not exist
 */
int getUser(UserInfo *user){
    int len = sizeof(_UserFromDB_)/sizeof(UserInfo);
    for(int i = 0; i < len; ++i){
        if(_UserFromDB_[i]->user_id == user->user_id){
            return USER_EXIST;
        }
    }
    /*
    if(len < MAX_USER){
        _UserFromDB_[LEN]->user_id = user->user_id;
    }*/
    return USER_NOT_EXIST;
}
int userCreate(UserInfo *user){
    int ret = getUser(user->user_id);
    if(ret == USER_NOT_EXIST){
        int len = sizeof(_UserFromDB_)/sizeof(UserInfo);
        if(len >= MAX_USER-1){
            return USER_TO_MUCH;
        }
        _UserFromDB_[len]->user_id = user->user_id;
        strcpy(_UserFromDB_[len]->user_name, user->user_name);
        return SUCCESS;
    }else{
        return USER_EXIST;
    }
}

int userDelete(UserInfo* user){
    int ret = getUser(user->user_id);
    if(index == USER_EXIST){
        _UserFromDB_[index]->user_id = USER_NEED_DELETE;
    }else{
        return USER_NOT_EXIST;
    }
}

int main(){
    
    return 0;
}

#include "friend_info.h"


int add_friend(UserInfo *self,UserInfo* other){
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);
    if(exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST){
        // check conflict
        int exist_friend = getFriendInfo(self->user_id, other->user_id);
        if(exist_friend == ALREADY_FRIEND){
            return ALREADY_FRIEND;
        }
        int add_ret = add_friend_info(self->user_id, other->user_id);
        return add_ret;
    }
    return USER_NOT_EXIST;
}

int delete_friend(UserInfo *self,UserInfo* other){
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);
    if(exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST){
        // check conflict
        int exist_friend = getFriendInfo(self->user_id, other->user_id);
        if(exist_friend){

            int ret = deleteFriendInfo(self->user_id, other->user_id);
            return ret;
            //return ALREADY_FRIEND;
        }else{
            return NOT_FRIEND;
        }
    }
    return USER_NOT_EXIST;
}

int add_black(UserInfo *self,UserInfo* other){
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);
    if(exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST){
        // check conflict
        int exist_black = getBlock(self->user_id, other->user_id);
        if(exist_black){
            return ALREADY_BLACK;
        }
        int ret = addBlack(self->user_id, other->user_id);
        return ret;
    }
    return USER_NOT_EXIST;
}

int delete_black(UserInfo *self, UserInfo* other){
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);
    if(exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST){
        // check conflict
        int exist_black = getBlock(self->user_id, other->user_id);
        if(exist_black){
            //return ALREADY_BLACK;
            int ret = deleteBlack(self->user_id, other->user_id);
            return ret;
        }else{
            return NOT_BLACK;
        }
    }
    return USER_NOT_EXIST;
}

#include"err_define.h"

#define MAX_USER 1024
#define MAX_FRIEND 1024

UserInfo *_UserFromDB_[MAX_USER];
FriendInfo *_FriendFromDB_[MAX_FRIEND];

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
    int friend_size = sizeof(_FriendFromDB_)/sizeof(FriendInfo);
    for(int i = 0; i < friend_size; ++i){
        if(_FriendFromDB_[i]->user_id_1 != FRIEND_NEED_DELETE && _FriendFromDB_[i]->user_id_1 != BLACK_NEED_DELETE){

        }
    }
    return 0;
}

/**
 * return user idx else return not exist
 */
int getUser(int user_id){
    int len = sizeof(_UserFromDB_)/sizeof(UserInfo);
    for(int i = 0; i < len; ++i){
        if(_UserFromDB_[i]->user_id == user_id){
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
    int index = getUser(user->user_id);
    if(index == USER_EXIST){
        _UserFromDB_[index]->user_id = USER_NEED_DELETE;
    }else{
        return USER_NOT_EXIST;
    }
}

int getFriendInfo(int self_id, int other_id, int type){
    int friend_size = sizeof(_FriendFromDB_)/sizeof(FriendInfo);
    for(int i = 0; i< friend_size; ++i){
        if(_FriendFromDB_[i]->user_id_1 == self_id && _FriendFromDB_[i]->user_id_2 == other_id && _FriendFromDB_[i]->type == type){
            return IS_FRIEND;
        }
    }
    if(type == FRIEND_TYPE) 
    return NOT_FRIEND;
    if(type == BLACK_TYPE)
    return NOT_BLACK;
    return TYPE_NOT_DEFINE;
}

int deleteFriendInfo(int self_id, int other_id){
    int friend_index = getFriendInfo(self_id,other_id,FRIEND_TYPE);
    if(friend_index != NOT_FRIEND){
        return NOT_FRIEND;
    }
    _FriendFromDB_[friend_index]->user_id_1 = FRIEND_NEED_DELETE;
    return SUCCESS;
}

int addFriendInfo(int self_id, int other_id){
    int friend_index = getFriendInfo(self_id,other_id,FRIEND_TYPE);
    if(friend_index != NOT_FRIEND){
        int size = sizeof(_FriendFromDB_)/sizeof(FriendInfo);
        if(size >= MAX_FRIEND){
            return FRIEND_TO_MUCH;
        }
        _FriendFromDB_[size]->user_id_1 = self_id;
        _FriendFromDB_[size]->user_id_2 = other_id;
        _FriendFromDB_[size]->type = 0;
        return SUCCESS;
    }
    
    return IS_FRIEND;
}
int getBlackInfo(int self_id, int other_id,int type){
    return getFriendInfo(self_id, other_id,type);
}
int addBlack(int self_id, int other_id){
    int black_index = getBlackInfo(self_id, other_id,BLACK_TYPE);
    if(black_index == NOT_BLACK){
        int size = sizeof(_FriendFromDB_)/sizeof(FriendInfo);
        if(size >= MAX_FRIEND-1){
            return BLACK_TO_MUCH;
        }
        _FriendFromDB_[size]->user_id_1 = self_id;
        _FriendFromDB_[size]->user_id_2 = other_id;
        _FriendFromDB_[size]->type = BLACK_TYPE;
        return SUCCESS;
        
    }
    return IS_BLACK;
}

int deleteBlack(int self_id, int other_id){
    int black_index = getFriendInfo(self_id,other_id,BLACK_TYPE);
    if(black_index != NOT_BLACK){
        return NOT_BLACK;
    }
    _FriendFromDB_[black_index]->user_id_1 = FRIEND_NEED_DELETE;
    return SUCCESS;

}

int main(){
    init();
    shutdown();


    return 0;
}

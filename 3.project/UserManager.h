#ifndef _USERMANAGER_H
#define _USERMANAGER_H
#include "UserInfo.h"

class UserManager{
private:
    UserInfo users[10240];
    GETSETVAR(int,user_count);
    void Init();
public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();
public:
    int ShowAll();
    UserInfo* GetUser(int user_id);
    int CreateUser(int user_id,char* user_name,char* pswd);
    int DeleteUser(int user_id);
};


#endif

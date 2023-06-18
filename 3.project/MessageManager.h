#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H
#include "MessageInfo.h"

class MessageManager{
private:
    MessageInfo messages_[256];
    GETSETVAR(int,message_count);

public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();
public:
    MessageInfo *GetMessage(int user_id,int message_id);
    MessageInfo *GetMessage(int message_id);
    int PushMessage(MessageInfo message);
    int DeleteMessage(int message_id);
};


#endif

#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H
#include "MessageInfo.h"
#include "mydb.h"

class MessageManager{
private:
    MessageInfo messages_[256];
    GETSETVAR(int,message_count);
    DbManager* db_svr_;
public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();

    int Init(DbManager* db_svr);
    void SavePhotos();
public:
    MessageInfo *GetMessageByUserId(int user_id);
    MessageInfo *GetMessageByMessageId(int message_id);
	int PublishMessage(MessageInfo message);
    int DeleteMessage(int message_id);
};


#endif

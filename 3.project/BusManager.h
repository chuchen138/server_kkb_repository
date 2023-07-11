/*************************************************************************
	> File Name: BusManager.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Fri 07 Jul 2023 06:36:09 PM CST
 ************************************************************************/

#ifndef _BUSMANAGER_H
#define _BUSMANAGER_H
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "pb/message_info_base.pb.h"
#include "pb/message_define.pb.h"
#include "MessageInfo.h"

#define TEXT_LEN 1024
struct messinfobuf 
{           
	messinfobuf():mtype(TEXT_LEN){}
	long mtype;       /* message type, must be > 0 */
    char mtext[TEXT_LEN];    /* message data */
};

class BusManager
{
private:
	/* data */
public:
	// BusManager(/* args */);
	// ~BusManager();
	int AddBusMessage(MessageInfo messinfo);
	int GetBusMessage(int user_id, sxg::GetMessageRsp *messinfo);
	int DeleteBusMessage(int user_id);
};


#endif

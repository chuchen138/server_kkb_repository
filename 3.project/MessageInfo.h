#ifndef _MESSAGEINFO_H
#define _MESSAGEINFO_H
#include "common/my_micro.h"
#include "string.h"
#include "pb/MessageInfoBase.pb.h"

class MessageInfo{
    GETSETVAR(int,publisher)
    GETSETVAR(int,message_id)
    GETSETSTR(10240,content)
	GETSETVAR(int,publish_time)
};


#endif

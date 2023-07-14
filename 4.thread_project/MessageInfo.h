#ifndef _MESSAGEINFO_H
#define _MESSAGEINFO_H
#include "common/my_micro.h"
#include "string.h"
#include "./pb/message_info_base.pb.h"

class MessageInfo{
    GETSETSTR(256,publisher)
    GETSETVAR(int,sublisher)
    GETSETVAR(int,message_id)
    GETSETSTR(4096,content)
	GETSETVAR(int,publish_time)
	GETSETVAR(int,db_flag)
};

#endif

/*************************************************************************
	> File Name: worker.h
	> Author: 孙相国
	> Mail: 3193088097@qq.com
	> Created Time: Tue 11 Jul 2023 11:37:54 PM CST
 ************************************************************************/

#ifndef _WORKER_H
#define _WORKER_H
#include "head.h"
#include "../pb/message_define.pb.h"
#include "../pb/message_info_base.pb.h"
#include "../pb/photo_info_base.pb.h"
#include "../pb/relation_info_base.pb.h"
#include "../pb/user_info_base.pb.h"
#include "../UserManager.h"
#include "../PhotoManager.h"
#include "../RelationManager.h"
#include "../MessageManager.h"
#include "../BusManager.h"
#include <unordered_map>

void *thread_work(void *arg);

#endif

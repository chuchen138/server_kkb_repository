#ifndef _RELATIONINFO_H
#define _RELATIONINFO_H
#include "common/my_micro.h"
#include "string.h"
#include "pb/relation_info_base.pb.h"

using namespace sxg;

class RelationInfo{
public:
    void FromPb(sxg::RelationInfoBase& relation);
    void ToPb(sxg::RelationInfoBase& relation);

private:
    GETSETVAR(int,user_id);
    int friend_list_[10240];
    int black_list_[10240];
public:
    int *getFriendList();
    int *getBlackList();
    int CheckFriend(int other_id);
    int CheckBlack(int other_id);
    int AddFriend(int other_id);
    int AddBlack(int other_id);
    int DeleteFriend(int other_id);
    int DeleteBlack(int other_id);
};

#endif

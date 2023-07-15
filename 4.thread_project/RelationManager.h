#ifndef _RELATIONMANAGER_H
#define _RELATIONMANAGER_H
#include "common/my_micro.h"
#include "mydb.h"
#include "RelationInfo.h"

class RelationManager{
private:
    RelationInfo relations_[1024];
    GETSETVAR(int,relation_count);
    DbManager* db_svr_;
    std::unordered_map<int,int> Id2Rela_idx;
public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();

    int Init(DbManager* db_svr);
    void SaveRelation();
    int DeleteRela(int user_id);

    RelationInfo* GetRelation(int user_id);
	int UserRelationInit(int user_id);
    int AddFriend(int user_id,int other_id);
    int DeleteFriend(int user_id,int other_id);
    int AddBlack(int user_id,int other_id);
    int DeleteBlack(int user_id,int other_id);

    int CheckFriend(int user_id,int check_id);
    int CheckBlack(int user_id,int check_id);
};

#endif

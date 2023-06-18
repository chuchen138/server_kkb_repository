#include "common/ret_value.h"
#include "RelationManager.h"
#include <stdio.h>

void RelationManager::Start(){
	printf("Start\n");
}
void RelationManager::Proc(){
	printf("Proc\n");
}

void RelationManager::Shutdown(){
	printf("Shutdown\n");
}

void RelationManager::Restart(){
	printf("Restart\n");
}

RelationInfo* RelationManager::GetRelation(int user_id){
	for(int i=0;i<relation_count_;i++){
		if(relations_[i].user_id()==user_id){
			return &relations_[i];
		}
	}
	return NULL;
}

int RelationManager::AddFriend(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	int ret=relation->AddFriend(other_id);
	return ret;
}

int RelationManager::DeleteFriend(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	int ret=relation->DeleteFriend(other_id);
	return ret;
}

int RelationManager::AddBlack(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	int ret=relation->AddBlack(other_id);
	return ret;
}

int RelationManager::DeleteBlack(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
        if(relation==NULL){
                return RELATION_NOT_FOUND;
        }
        int ret=relation->DeleteBlack(other_id);
        return ret;
}


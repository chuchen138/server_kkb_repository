#include "common/ret_value.h"
#include "RelationManager.h"
#include <stdio.h>

int RelationManager::Init(DbManager* db_svr){
    db_svr_ = db_svr;
	printf("RelationManager Init\n");
	return SUCCESS;
}


void RelationManager::Start(){
	int ret=db_svr_->GetRelasBegin();
	int ui=0;
	if(ret==SUCCESS){
		ret=db_svr_->GetRelasOneByOne(&relations_[ui]);
		if(ret==SUCCESS)ui++;
		while(ret!=DB_NO_MORE_DATA){
			ret=db_svr_->GetRelasOneByOne(&relations_[ui]);
			if(ret==SUCCESS)ui++;
		}
		ret=db_svr_->GetRelasEnd();
		set_relation_count(ui);
	}else{
		printf("[Error   ]RelationManager Start fail\n");
		return;
	}
	printf("========================================\n");
	for(int i=0;i<relation_count();i++){
		printf("| user_id: %d | friend : ",relations_[i].user_id());
		for(int j=0;j<relations_[i].friend_count();j++){
			printf("%d ",relations_[i].friend_list_[j]);
		}
		printf("| black : ");
		for(int j=0;j<relations_[i].black_count();j++){
			printf("%d \n",relations_[i].black_list_[j]);
		}
		printf("\n");
	}
	printf("========================================\n");
	printf("RelationManager::Start Over\n");
}
void RelationManager::Proc(){
	SaveRelation();
	// printf("RelationManager::Proc\n");
}

void RelationManager::Shutdown(){
	SaveRelation();
	// printf("RelationManager::Shutdown\n");
}

void RelationManager::Restart(){
	SaveRelation();
	Start();
	printf("RelationManager::Restart\n");
}

int RelationManager::UserRelationInit(int user_id){
	relations_[relation_count_].set_user_id(user_id);
	relations_[relation_count_].set_db_flag(FLAG_INSERT);
	set_relation_count(relation_count()+1);
    return 0;
}

RelationInfo* RelationManager::GetRelation(int user_id){
	for(int i=0;i<relation_count();i++){
		if(relations_[i].user_id()==user_id){
			return &relations_[i];
		}
	}
	return NULL;
}

int RelationManager::AddFriend(int user_id,int other_id){
	cout << "relation count:" << relation_count() << endl;
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	relation->set_db_flag(FLAG_UPDATE);
	int ret=relation->AddFriend(other_id);
relation = GetRelation(other_id);
relation->set_db_flag(FLAG_UPDATE);
relation->AddFriend(user_id);
	return ret;
}

int RelationManager::DeleteFriend(int user_id,int other_id){
	RelationInfo* relation=GetRelation(user_id);
	if(relation==NULL){
		return RELATION_NOT_FOUND;
	}
	relation->set_db_flag(FLAG_UPDATE);
	int ret=relation->DeleteFriend(other_id);
relation = GetRelation(other_id);
relation->set_db_flag(FLAG_UPDATE);
relation->DeleteFriend(user_id);
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

int RelationManager::DeleteRela(int user_id){
	for(int i=0;i<relation_count();i++){
		if(relations_[i].user_id()==user_id){
			printf("USER EXIST %d\n",relations_[i].user_id());
            relations_[i].set_db_flag(FLAG_DELETE);
			// users[i].set_user_id(-1);
			return USER_EXIST;
		}
	}
	return 0;
}

void RelationManager::SaveRelation(){
	for(int i=0;i< relation_count();i++){
		/* 1 delete */
		if(relations_[i].db_flag()==FLAG_DELETE){
			int user_id = relations_[i].user_id();
			for(int j=0;j < relations_[i].friend_count();j++){
				int other_id = relations_[i].friend_list_[j];
				if(CheckFriend(other_id,user_id) == ALREADY_FRIEND)
				DeleteFriend(user_id,other_id);

				if(CheckBlack(other_id,user_id) == ALREADY_BLACK)
				DeleteBlack(other_id,user_id);
			}
			for(int j=0;j < relations_[i].black_count();j++){
				int other_id = relations_[i].black_list_[j];
				if(CheckFriend(other_id,user_id) == ALREADY_FRIEND)
				DeleteFriend(other_id,user_id);
				
				if(CheckBlack(other_id,user_id) == ALREADY_BLACK)
				DeleteBlack(other_id,user_id);
			}
			db_svr_->DeleteRela(user_id);
			relations_[i].set_db_flag(FLAG_INIT);
		}
		/* 2 update */
		if(relations_[i].db_flag()==FLAG_UPDATE){
			db_svr_->UpdateRela(&relations_[i]);
                        relations_[i].set_db_flag(FLAG_INIT);
		}
		/* 3 insert */
		if(relations_[i].db_flag()==FLAG_INSERT){
			if(db_svr_->InsertRela(&relations_[i]) == SUCCESS){
				printf("Insert Rela %d \n",relations_[i].user_id());
			}else{
				printf("Insert Rela fail %d \n",relations_[i].user_id());
				return;
			}
			relations_[i].set_db_flag(FLAG_INIT);
		}
	}
	return;

}


int RelationManager::CheckFriend(int user_id,int check_id){
	for(int i=0;i<relation_count();i++){
		if(relations_[i].user_id()==user_id){
            return relations_[i].CheckFriend(check_id);
			// users[i].set_user_id(-1);
		}
	}
	return 0;
}
int RelationManager::CheckBlack(int user_id,int check_id){
	for(int i=0;i<relation_count();i++){
		if(relations_[i].user_id()==user_id){
            return relations_[i].CheckBlack(check_id);
			// users[i].set_user_id(-1);
		}
	}
	return 0;
}

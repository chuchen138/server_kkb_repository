#ifndef _PHOTOMESSAGE_H
#define _PHOTOMESSAGE_H
#include "PhotoInfo.h"
#include "mydb.h"

class PhotoManager {
private:
	PhotoInfo photos_[10240];
	GETSETVAR(int,photo_count)
    DbManager* db_svr_;

public:
	void Start();
	void Proc();
	void Shutdown();
	void Restart();

	int Init(DbManager* db_svr);
	void SavePhotos();
public:
	PhotoInfo* GetPhoto(int user_id);
	int UpdatePhoto(int user_id,const char* publisher,int publish_time,int publish_message_id);
};
#endif

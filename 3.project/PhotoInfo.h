#ifndef _PHOTOINFO_H
#define _PHOTOINFO_H
#include "common/my_micro.h"

#include "pb/PhotoInfoBase.pb.h"

class PhotoInfo {
public:
	void FromPb(sxg::PhotoInfoBase& photo);
	void ToPb(sxg::PhotoInfoBase& photo);
private:
	GETSETVAR(int,user_id)
	GETSETVAR(int,last_publisher)
	GETSETVAR(int,last_publish_time)
	GETSETVAR(int,last_publish_message_id)
};
#endif

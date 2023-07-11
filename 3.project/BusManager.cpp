#include "BusManager.h"

int BusManager::AddBusMessage(MessageInfo messinfo)
{
	sxg::MessageInfoBase messinfobase;
	messinfobase.set_content(messinfo.content());
	messinfobase.set_publisher(messinfo.publisher());
	messinfobase.set_sublisher(messinfo.sublisher());
	messinfobase.set_message_id(messinfo.message_id());
	messinfobase.set_publish_time(messinfo.publish_time());
	messinfobuf sendBuf;
	messinfobase.SerializeToArray(sendBuf.mtext,TEXT_LEN);
    int msgId = msgget(messinfo.sublisher(), IPC_CREAT|0777);
 
	if(msgId == -1 ){
		printf("get que failuer\n");
		return -1;
	}
	
	// msgsnd(msgId,&sendBuf,TEXT_LEN,0);//写入
	msgsnd(msgId,&sendBuf,sizeof(sendBuf.mtext),0);//写入
	// for(int i = 0; i < TEXT_LEN; i++){
	// 	printf("%c",sendBuf.mtext[i]);
	// }
// sxg::MessageInfoBase messtest1;
// messtest1.ParseFromArray(sendBuf.mtext,TEXT_LEN);
// printf("AddBusMessage send message:[%s] over\n",messtest1.content().c_str());
// memset(&sendBuf,0,sizeof(struct messinfobuf));
// msgrcv(msgId,&sendBuf,TEXT_LEN,TEXT_LEN,IPC_NOWAIT);
// sxg::MessageInfoBase messtest;
// messtest.ParseFromArray(sendBuf.mtext,TEXT_LEN);
// printf("AddBusMessage recv message:[%s] over\n",messtest.content().c_str());
	
	return 0;
}

int BusManager::GetBusMessage(int user_id, sxg::GetMessageRsp *messinfo){
	int msgId = msgget(user_id, IPC_CREAT|0777);
	
	if(msgId == -1 ){
        printf("get que failuer\n");
    }
    messinfobuf readBuf;
	memset(&readBuf,0,sizeof(struct messinfobuf));
	int ret = 1, count = 0;
	while(ret){
		ret = msgrcv(msgId, &readBuf,sizeof(readBuf.mtext),TEXT_LEN,IPC_NOWAIT);//接收
		if(ret <= 0) break;
		sxg::MessageInfoBase *messtmp = messinfo->add_message_list();
		messtmp->ParseFromArray(readBuf.mtext,TEXT_LEN);
		printf("GetBusMessage receive message:[%s] over\n",messtmp->content().c_str());
		count++;
	}
	return count;
}

int BusManager::DeleteBusMessage(int user_id){
	int msgId = msgget(user_id, IPC_CREAT|0777);
	if(msgId == -1 ){
		printf("get que failuer\n");
		return -1;
	}
	msgctl(msgId,IPC_RMID,NULL);
	return 0;
}

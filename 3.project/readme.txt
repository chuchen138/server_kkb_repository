时间：6月8日
服务器部分：简单的收发工作
  server: socket->bind->listen->accept->write->close;
  client: socket->bind->connect->read->close
关系部分: user friend
user_info.h
  UserInfo:
    user_id,name,reg_time,login_time and so on;
  function:
      user_create
      user_login
      user_logout
      user_delete
friend_info.h
  FriendInfo
    user_id_1,user_id_2,type;
  UserBrief
    user_id,user_name;
  FriendList
    user_id,UserBrief* friendList,UserBrief* blackList;

    function:
      add_friend
      delete_friend
      add_black
      delete_black

时间：6月11日
主要完成工作：大体框架完成
添加文件 
    UserInfo.h 
        读取回写数据库内容，定义用户参数
    UserManager.h UserManager.cpp
        建立一个用户信息池，可创建用户，删除用户，根据用户ID获取
    RelationInfo.h RelationInfo.cpp
        好友列表，黑名单列表；得到好友信息，黑名单信息，
        检查是否是好友，检查是否是黑名单；添加或删除好友黑名单
    RelationManager.cpp RelationManager.h
        关系池;得到关系;添加或删除黑名单或好友。
    MessageInfo.h
        用户ID，消息ID，消息存储
    MessageManager.h MessageManager.cpp
        MessageInfo池，根据用户ID或消息ID得到MessageInfo.h
        PushMessage DeleteMessage
    PhotoInfo.h
        用户ID，最新发布者，最新发布时间，最新发布消息ID
    PhotoManager.h PhotoManager.cpp
        PhotoInfo池，根据用户ID得到PhotoInfo
        根据PhotoInfo信息跟新消息

更改 MakeFile 可以编译以上文件
快照就是收到的消息队列，可以从快照拉出收到的消息


user_info.h
    void FromPb(sxg::UserInfoBase& user);
    void ToPb(sxg::UserInfoBase& user);
    GETSETVAR(int,user_id)
    GETSETSTR(256,user_name)
    GETSETSTR(256,nick_name)
    GETSETVAR(int,reg_time)
    GETSETVAR(int,from)
    GETSETVAR(int,login_time)
    GETSETVAR(int,last_login_time)
    GETSETVAR(int,fresh_time)
    GETSETSTR(256,password)

UserManager.h
    userInfo users[10240]; // 用户数据存储
    GETSETVAR(int,user_count);
    void Init();
    public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();
    int ShowAll();
    UserInfo* GetUser(int user_id); // 传入ID从用户信息池中得到用户信息的类
    int CreateUser(int user_id,char* user_name,char* pswd); // 建立用户
    int DeleteUser(int user_id); // 删除用户 根据用户ID

RelationInfo.h
    private:
    GETSETVAR(int,user_id);
    int friend_list_[10240];
    int black_list_[10240];
    public:
    int *getFriendList();
    int *getBlackList();
    int CheckFriend(int other_id); // 遍历好友列表，判断other_id是否为好友
    int CheckBlack(int other_id); // 遍历黑名单列表，判断other_id是否在黑名单
    int AddFriend(int other_id);
    int AddBlack(int other_id);
    int DeleteFriend(int other_id);
    int DeleteBlack(int other_id);

RelationManager.h
    private:
    RelationInfo relations_[10240];
    GETSETVAR(int,relation_count);
    RelationInfo* GetRelation(int user_id);
    public:
    int AddFriend(int user_id,int other_id);
    int DeleteFriend(int user_id,int other_id);
    int AddBlack(int user_id,int other_id);
    int DeleteBlack(int user_id,int other_id);

MessageInfo.h
    GETSETVAR(int,user_id)
    GETSETVAR(int,message_id)
    GETSETSTR(10240,content)

MessageManager.h
    MessageInfo messages_[256];
    GETSETVAR(int,message_count);
    public:
    MessageInfo *GetMessage(int user_id,int message_id);
    MessageInfo *GetMessage(int message_id);
    int PushMessage(MessageInfo message);
    int DeleteMessage(int message_id);

快照就是收到的消息队列，可以从快照拉出收到的消息
PhotoInfo.h
    GETSETVARR(int,user_id)
    GETSETVAR(int,last_publisher)
    GETSETVAR(int,last_publish_time)
    GETSETVAR(int,last_publish_message_id)

PhotoManager.h
    private:                    
    PhotoInfo photos_[10240];
    GETSETVAR(int,photo_count)
    public:
    
    PhotoInfo* GetPhoto(int user_id);
    int UpdatePhoto(int user_id,int publisher_id,int publish_time,int publish_message_id);



6月13日
主要完成工作：
开始加入数据库, 添加测试文件 mydbtest.cpp mydb.cpp mydb.h
为UserManager类完善成员方法。
pb 添加文件 message_define.proto 定义消息类型，发送接收参数
common 添加文件 mess_type.h 消息返回参数
user_info_base中的全部变量：
    user_id,user_name,nick_name,reg_time,from,login_time,
    last_login_time,fresh_time,password,logout_time;
UserManager类中
    成员变量中加入数据库的参数。
    Start方法，调用数据库，从数据库中得到用户数据。
    Shutdown方法，写入数据库。
    CheckEcist(int user_id); // 判断是否存在用户
    添加LoginCheck(char *user_name,char *user_pswd);通过用户名和密码检测用户的方法；
    添加getUserIDbyname();// 通过用户姓名得到ID；
    int UpdateUserLoginTime(int user_id, int time_now); // 更新登录时间
	int UpdateUserFreshTime(int user_id, int time_now); // 更新刷新时间
UserInfo 类中
    添加数据库相关变量（更新，删除）
    退出时间
RelationInfo类中添加
	int GetFriendUserIdByIndex(int index);
	int GetBlackUserIdByIndex(int index);
RelationManager类中
    int UserRelationInit(int user_id); // 关系初使化
    

6月14号
主要完成工作，部分测试
recv_buffer
send_buffer
Test() 测试随便选择用户名和密码建立用户，生成UserInfo
getMessType()  得到消息类型，前三位。
SocketInit()中socket 初始化。
SocketAccept() 中建立连接。
ClientClose()  客户端关闭。
SocketCheckRecv() 网络连接中接收消息。
SocketSendRsp()   网络恢复消息
SocketClose()     服务器关闭。
SetRspMessType()  恢复消息类型。
main函数中，
    随机使用用户名密码生成登录
    在死循环中，一直监听收到的信息。
    通过得到的消息类型进行不同处理。


6月15日
主要完成任务，完成客户端测试。
随机生成用户名密码，连接服务器。
send_buffer
recv_buffer
ProtoInit() 各种消息类型初始化
SetReqMessType() 发送消息类型
GetMessType() 
RecvRsp() // 接收回复
ClientSocketInit() // 建立连接

6月16日
主要任务：更改大体框架，完成数据库连接。
删掉管理文件的SQL服务。
完善mydb.h的数据库接口。
mydb.h 添加类成员方法
    int GetUserId(); // 从数据库中得到需要的用户ID。
	int GetUsersBegin(); // 得到开始的用户
	int GetUsersOneByOne(UserInfo* user); // 将用户一个个加入用户池；
	int GetUsersEnd(); // 得到最后的用户
	int InsertUser(UserInfo* user); // 在数据库插入用户信息
	int UpdateUser(UserInfo* user); 
	int DeleteUser(UserInfo* user);
    

6月20日
主要任务：共享内存通信
添加测试文件ShmSetter.cpp

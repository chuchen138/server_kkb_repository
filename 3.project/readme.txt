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
大体框架完成
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
    userInfo users[10240];
    GETSETVAR(int,user_count);
    void Init();
    public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();
    int ShowAll();
    UserInfo* GetUser(int user_id);
    int CreateUser(int user_id,char* user_name,char* pswd);
    int DeleteUser(int user_id);

RelationInfo.h
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

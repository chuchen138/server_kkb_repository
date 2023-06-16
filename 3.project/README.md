时间：6月16日
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


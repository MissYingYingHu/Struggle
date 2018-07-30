#pragma once

#include <string>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include"block_queue.h"
#include "api.h"


namespace server
{

struct Content
{
    std::string str;
    sockaddr_in addr;
};

class ChatServer
{
public:
    int Start(const std::string& ip,short port);

    //接收消息
    int RecvMsg();

    //广播消息
    int BroadMsg();

    //添加好友
    void AddUser(sockaddr_in addr,const std::string& name);

    //删除好友
    void DelUser(sockaddr_in addr,const std::string& name);

    //发送数据
    void SendMsg(const std::string& data,sockaddr_in addr);

private:
    static void* Consume(void*);
    static void* Product(void*);
    //key:ip + 昵称
    //value:ip + port
    std::unordered_map<std::string,sockaddr_in> online_friend_list;
    //实现一个队列作(使用阻塞队列)为交易场所
    BlockQueue<Content> queue;
    int sock;   //服务器的文件描述符保存起来

};

}//end server


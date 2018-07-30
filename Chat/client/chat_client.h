#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include "../server/api.h"
namespace client
{

class ChatClient{

public:
  //构造函数没有返回值，就无法判断是否构造成功。---》解决：自己写一个初始化的函数
  //ChatClient();
  //客户端初始化
  int init_client(const std:: string& ip,short port);

  //设置用户信息:让客户端启动的时候，从标准输入读取数据，传给下面的函数
  int UserInfo(const std::string& _name,const std::string& _school);

  //发送消息
  void SendMsg(const std::string& msg);

  //接收消息
  void RecvMsg(server::Data* data);

private:
  int sock;  //与服务器交互的文件描述符
  struct sockaddr_in server;
  std::string name;
  std::string school;
}; 

}//end client

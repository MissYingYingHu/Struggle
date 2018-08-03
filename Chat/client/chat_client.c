#include "chat_client.h"

namespace client{

  //IP和port通过命令行参数来传递=======》优化：从配置文件之中读取IP和port？？？
  //启动时读取配置文件。比如可以使用json组织配置格式:然后使用jsoncpp进行解析
  int ChatClient::init_client(const std::string& ip,short port)
  {
     sock = socket(AF_INET,SOCK_DGRAM,0);
     if(sock < 0)
     {
       perror("socket");
       return -1;
     }
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = inet_addr(ip.c_str());
     server.sin_port = htons(port);
     //printf("client socket\n");
     return 0;
  }
     
  int ChatClient::UserInfo(const std::string& _name,const std::string& _school)
  {
     name = _name;
     school = _school;
     return 0;
  }

  void ChatClient::SendMsg(const std::string& msg)
  {
     //在发送数据时，要把用户的名字也要加上
     //sendto(sock,msg.c_str(),msg.size(),0,(sockaddr*)&server,sizeof(server));
     server::Data data;
     data.name = name;
     data.school = school;
     data.msg = msg;
     //下线的判断
     if(data.msg == "quit" || data.msg == "exit")
     {
       data.command = "quit";
     }
     std::string str;
     data.Serialize(&str);
     //发送数据
     //std::cout<<str.data()<<" "<<str.size()<<std::endl; 
     ssize_t send_size = sendto(sock,str.data(),str.size(),0,(struct sockaddr*)&server,sizeof(server));
     if(send_size < 0)
     {
       printf("send_size:%lu\n",send_size);
       perror("sendto");
       return ;
     }
  }
  
  void ChatClient::RecvMsg(server::Data* data)
  {
      char buf[1024*10] = {0};
      ssize_t read_size = recvfrom(sock,buf,sizeof(buf) - 1,0,NULL,NULL);
      if(read_size < 0)
      {
        perror("recvfrom");
        return ;
      }
      buf[read_size] = '\0';
      data->UnSerialize(buf);
  }

}



#ifdef CHAT
//////////////////test////////////////

void* Send(void* arg)
{
  client::ChatClient* client = reinterpret_cast<client::ChatClient*>(arg);
  while(true)
  {
    std::string str;
    std::cin>>str;
    client->SendMsg(str);
  }
  return NULL;
}

void* Recv(void* arg)
{
  client::ChatClient* client = reinterpret_cast<client::ChatClient*>(arg);
  while(true)
  {
    server::Data data;
    client->RecvMsg(&data);
    std::cout<<"["<<data.name<<":=>"<<data.school<<"]"<<data.msg<<"\n";
  }
  return NULL;
}

int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("usage:./client [ip] [port]");
    return 1;
  }
  client::ChatClient client;
  client.init_client(argv[1],atoi(argv[2]));
  std::string name;
  std::string school;
  std::cout<<"输入用户名：";
  std::cin>>name;
  std::cout<<"输入学校：";
  std::cin>>school;
  client.UserInfo(name,school);

  //创建线程分别做不同的事情
  pthread_t send_tid;
  pthread_t recv_tid;
  pthread_create(&send_tid,NULL,Send,&client);
  pthread_create(&recv_tid,NULL,Recv,&client);
  pthread_join(send_tid,NULL);
  pthread_join(recv_tid,NULL);

  return 0;
}

#endif 

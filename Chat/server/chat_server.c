#include "chat_server.h"

namespace server
{
int ChatServer::Start(const std::string& ip,short port)
{
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0)
    {
        perror("socket");
        return 1;
    }
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    server_addr.sin_port = htons(port);
    int ret = bind(sock,(sockaddr*)&server_addr,sizeof(server_addr));
    if(ret < 0)
    {
        perror("bind");
        return 1;
    }
    pthread_t productor,consumer;
    pthread_create(&productor,NULL,Product,this);
    pthread_create(&consumer,NULL,Consume,this);
    pthread_join(productor,NULL);
    pthread_join(consumer,NULL);

    return 0;
}

void* ChatServer::Product(void* arg)
{
    //生产者线程:读取socket中的数据并且写入到BlockQueue中
    ChatServer* chat = reinterpret_cast<ChatServer*>(arg);
    while(true)
    {
        chat->RecvMsg();
    }
    printf("Product\n");
    return NULL;
}

void* ChatServer::Consume(void* arg)
{
    //消费者
    ChatServer* chat = reinterpret_cast<ChatServer*>(arg);
    while(true)
    {
        chat->BroadMsg();
    }
    return NULL;
}

int ChatServer::RecvMsg()
{
    char buf[1024*3] = {0};
    sockaddr_in peer;
    socklen_t len = sizeof(peer);
    //1.从socket中读取数据
    ssize_t recv_size = recvfrom(sock,buf,sizeof(buf) - 1,0,(sockaddr*)&peer,&len);
    if(recv_size < 0)
    {
        perror("recvfrom");
        return -1;
    }
    //2.写到BlockQueue中
    Content con;
    con.str = buf;
    con.addr = peer;
    queue.PushBack(con);    
    return 0;
}

int ChatServer::BroadMsg()
{
    Content con;
    //1.从队列中读取元素
    queue.PopFront(&con);
    //2.将取出的字符串数据进行反序列化
    Data data;
    data.UnSerialize(con.str);
    //3.根据消息更新在线成员列表
    if(data.command == "quit")
    {
        //b)若是一个下线消息,就把该成员从列表中删除
        DelUser(con.addr,data.name);
    }
    else
    {
        //a)如果这个成员不在当前好友列表中,加入进来
        AddUser(con.addr,data.name);
    }
    //4.遍历好友列表,将消息发送给每一个成员
    for(auto item : online_friend_list)
    {
        SendMsg(con.str,item.second);
    }
    return 0;
}

void ChatServer::AddUser(sockaddr_in addr,const std::string& name)
{
    //构造key和value
    //可以:name+IP
    //value:IP
    std::stringstream ss;
    ss<< name <<":"<<addr.sin_addr.s_addr;

    //如果不存在,那么就插入
    //如果存在,那么就更新(修改)
    online_friend_list[ss.str()] = addr;
}

void ChatServer::DelUser(sockaddr_in addr,const std::string& name)
{
    std::stringstream ss;  //把标准输出输出到字符串中
    ss<<name<<":"<<addr.sin_addr.s_addr;
    online_friend_list.erase(ss.str());
}

void ChatServer::SendMsg(const std::string& str,sockaddr_in addr)
{
    //1.把数据进行序列化
    //2.把数据发送给客户端
    //把数据通过 sendto 发送给客户端
    sendto(sock,str.data(),str.size(),0,(sockaddr*)&addr,sizeof(addr));
}

}

#include <signal.h>
#include "chat_client.h"
#include "../server/api.h"
#include "windows.h"

client::ChatClient* global_client = NULL;
client::Window* global_window = NULL;
pthread_mutex_t global_lock;

void* Send(void *arg)
{
  (void)arg;
  while(1)
  {
    std::string msg;
    pthread_mutex_lock(&global_lock);
    global_window->Draw_Input();
    pthread_mutex_unlock(&global_lock);
    global_window->GetStringFromPicture(global_window->input_win,&msg);
    global_client->SendMsg(msg);
  }
  return NULL;
}

void* Recv(void *arg)
{
  (void)arg;
  while(1)
  {
    pthread_mutex_lock(&global_lock);
    global_window->Draw_Output();
    pthread_mutex_unlock(&global_lock);
    global_window->DrawFriendList();
    server::Data data;
    global_client->RecvMsg(&data);
    if(data.command == "quit" || data.command == "exit")
    {
      global_window->DelFriend(data.name + ":" + data.school);
    }
    else
    {
      global_window->AddFriend(data.name + ":"+data.school);
      global_window->AddMsg(data.name + "=>" + data.msg);
    }
  }
  return NULL;
}

void Exit(int sig)
{
  (void)sig;
  //主要为了调用window的析构函数，如果不进行析构，就可能会显示混乱
  global_client->SendMsg("quit");
  delete global_client;
  delete global_window;
  exit(0);
}

void StartRun(const std::string& ip,short port)
{
  signal(SIGINT,Exit);
  pthread_mutex_init(&global_lock,NULL);
  //初始化客户端
  //提示用户输入
  //初始化界面
 global_client = new client::ChatClient();
 int ret = global_client->init_client(ip,port);
 if(ret < 0)
 {
   printf("client init failed.\n");
   return ;
 }
 std::string name,school;
 std::cout<<"please input your name:"<<std::endl;
 std::cin>>name;
 std::cout<<"please input your school:"<<std::endl;
 std::cin>>school;

 global_client->UserInfo(name,school);
 global_window = new client::Window();
 global_window->Draw_Header();
  //创建2个线程，负责数据的读取和发送
      //1）发送：获取输入并发给服务器
      //2）读取：从服务器获取数据，并显示到界面
  pthread_t send_tid,read_tid;
  pthread_create(&send_tid,NULL,Send,NULL);
  pthread_create(&read_tid,NULL,Recv,NULL);
  pthread_join(send_tid,NULL);
  pthread_join(read_tid,NULL);
  //结尾
  delete global_client;
  delete global_window;
  pthread_mutex_destroy(&global_lock);
}
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    printf("Usage: ./client [ip] [port]\n");
    return 1;
  }
  StartRun(argv[1],atoi(argv[2]));
  return 0;
}

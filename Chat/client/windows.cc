#include "windows.h"
#include <locale.h>
#include "../server/api.h"

namespace client
{

Window::Window()
{
    //设置编码字符
    setlocale(LC_ALL,"");
    
    //初始化
    initscr();

    //隐藏光标
    curs_set(0);
}


Window::~Window()
{
    //对整个客户端进行销毁
    endwin();
}

void Window::Draw_Header()
{   
    //LINES：表示当前窗口的最大长度
    int height = LINES/5;
    int width = COLS;
    int y = 0;
    int x = 0;
    header_win = newwin(height,width,y,x);
    std::string titile = "我是一个聊天的窗口";
    PutStringToPicture(header_win,height/2,width/2 - titile.size()/2,titile);
    box(header_win,'|','=');
    wrefresh(header_win);
}

void Window::PutStringToPicture(WINDOW* win,int y,int x,const std::string& str)
{
    mvwaddstr(win,y,x,str.c_str());
}
 
void Window::GetStringFromPicture(WINDOW* win,std::string* str)
{
    char buf[1024*5] = {0};
    wgetnstr(win,buf,sizeof(buf) - 1);
    *str = buf;
}

void Window::Draw_Input()
{
    int height = LINES/5;
    int width = COLS;
    int y = LINES*4 / 5;
    int x = 0;
    input_win = newwin(height,width,y,x);
    std::string titile = "please input:";
    PutStringToPicture(input_win,1,2,titile);
    box(input_win,'|','=');
    wrefresh(input_win);
}

void Window::Draw_Output()
{
    int height = LINES * 3/5;
    int width = COLS * 3/4;
    int y = LINES / 5;
    int x = 0;
    output_win = newwin(height,width,y,x);
    box(output_win,'*','=');
    size_t i = 0;
    for(i = 0;i < msg.size();++i)
    {
      PutStringToPicture(output_win,i+1,2,msg[i]);
    }
    wrefresh(output_win);
}

void Window::AddMsg(const std::string& msgs)
{
    //当显示的消息的数量超过10条时，就把旧的消息删除掉
    msg.push_back(msgs);
    int len = LINES * 3/5 - 2;
    if((int)msg.size() > len)
    {
      msg.pop_front();
    }
}

void Window::AddFriend(const std::string& friend_list_info)
{
 friend_list.insert(friend_list_info); 
}

void Window::DrawFriendList()
{
  int height = LINES * 3/5;
  int width = COLS / 4;
  int y = LINES / 5;
  int x = COLS * 3 / 4 + 1;
  friend_list_win = newwin(height,width,y,x);
  box(friend_list_win,'|','=');
  size_t i = 0;
  for(auto item:friend_list)
  {
    PutStringToPicture(friend_list_win,i + 1,1,item);
    ++i;
  }
  wrefresh(friend_list_win);
}

void Window::DelFriend(const std::string& friend_list_info)
{
  friend_list.erase(friend_list_info);
}

}

#ifdef WIN
int main()
{
  client::Window win;
  win.Draw_Header();
  win.Draw_Input();
  win.AddMsg("me:hello");
  win.AddMsg("you:hello");
  win.AddMsg("he:hello");
  win.Draw_Output();
  win.AddFriend("ha:eur");
  win.AddFriend("hq:eur");
  win.AddFriend("hs:eur");
  win.DrawFriendList();
  sleep(5);
  return 0;
}
#endif 

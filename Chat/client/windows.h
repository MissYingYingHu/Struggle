#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <deque>
#include <unordered_set>

namespace client
{

class Window
{
public:

  Window();

  ~Window();
  
  //窗口名称
  void Draw_Header();

  //输入框
  void Draw_Input();

  //输出框
  void Draw_Output();
  
  //好友列表框
  void DrawFriendList();

  //往窗口中写数据
  void PutStringToPicture(WINDOW* win,int y,int x,const std::string& str);

  //从窗口中获取数据
  void GetStringFromPicture(WINDOW* win,std::string* str);

  void AddMsg(const std::string& msgs);
  void AddFriend(const std::string& friend_list_info);
  void DelFriend(const std::string& friend_list_info);

  WINDOW* header_win;
  WINDOW* input_win;
  WINDOW* output_win;
  WINDOW* friend_list_win;

private:

  std::deque<std::string> msg;
  std::unordered_set<std::string> friend_list;

};

}

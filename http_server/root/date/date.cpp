#include<iostream>
#include<unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"date.h"

bool Date:: IsLeapYear(int year)
{
  if((year % 4 == 0 && year % 100 != 0) && (year % 400 == 0))
  {
    return true;
  }
  return false;
}

bool Date::IsInvalid()
{
  if(this->_year < 1 || this->_month > 12 || this->_month < 1 || this->_day < 1 || this->_day > GetCorrentDay(this->_year,this->_month))
  {
    return false;
  }
  return true;
}

int Date::GetCorrentDay(int year,int month)
{
  int tmp_day;
  int day[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
  if(month == 2 && IsLeapYear(year))
  {
    tmp_day = 29;
  }
  else
  {
    tmp_day = day[month];
  }
  return tmp_day;
}

bool Date::operator<(const Date& d)
{
  if(_year < d._year || (_year == d._year && _month < d._month) || (_year == d._year && _month == d._month && _day < d._day))
  {
    return true;
  }
  return false;
}

bool Date::operator==(const Date& d)
{
  if(_year == d._year && _month == d._month && _day == d._day)
  {
    return true;
  }
  return false;
}

bool Date::operator!=(const Date& d)
{
  return !(*this == d);
}

Date& Date::operator++()
{
  *this += 1;
  return *this;
}

Date Date::operator+(const int day)
{
  Date tmp(*this);
  tmp += day;
  return tmp;
}

Date& Date::operator+=(const int day)
{
  if(day < 0)
  {
    return *this -= (-day);
  }
  _day += day;
  while((*this).IsInvalid() == false)
  {
    if(_month == 13)
    {
      _year += 1;
      _month = 1;
    }
    int corrent_day = GetCorrentDay(_year,_month);
    _day -= corrent_day;
    _month += 1;
  }
  return *this;
}

Date Date::operator-(int const day)
{
  Date tmp(*this);
  tmp -= day;
  return *this;
}

Date& Date::operator-=(const int day)
{
  if(day < 0)
  {
    return *this += (-day);
  }
  while(_day < day)
  {
    if(_month == 1)
    {
      _year -= 1;
      _month = 12;
    }
    else
    {
      _month -= 1;
    }
    _day += GetCorrentDay(_year,_month);
  }
  _day -= day;
  return *this;
}

int Date::operator-(Date& d)
{
  int count = 0;
  while(*this != d)
  {
    if(*this < d)
    {
      ++(*this);
      ++count;
    }
    else
    {
      ++d;
      ++count;
    }
  }
  return count;
}

void Date::PrintDate()
{
  std::cout<<_year<<"-"<<_month<<"-"<<_day<<std::endl;
}

//对于收到的请求进行计算处理
static int GetRequest(char buf[])
{
  //1.首先要获取到环境变量
  char* method = getenv("REQUEST_METHOD");
  if(method == NULL)
  {
    fprintf(stderr,"method=NULL\n");
    return -1;
  }
  //2.对于不同的环境变量进行不同的处理
  if(strcasecmp(method,"GET") == 0)
  {
    char* quary = getenv("QUERY_STRING");
    if(quary == NULL)
    {
      fprintf(stderr,"quary=NULL\n");
      return -1;
    }
    strcpy(buf,quary);
  }
  else
  {
    char* content_length = getenv("CONTENT_LENGTH");
    if(content_length == 0)
    {
      fprintf(stderr,"content_length=0\n");
      return -1;
    }
    int length = atoi(content_length);
    char c = '\0';
    int i = 0;
    for(;i < length;++i)
    {
      read(0, &c, 1);
      buf[i] = c;
    }
    buf[length] = '\0';
  }
  return 0;
  
}

int main()
{
  char buf[1024*10] = {0};
  int ret = GetRequest(buf);
  if(ret < 0)
  {
    fprintf(stderr,"\nGetRequest failed.");
    return 1;
  }
  Date d1;
  Date d2;
  sscanf(buf,"year=%d&month=%d&day=%d&year=%d&month=%d&day=%d",&d1._year,&d1._month,&d1._day,&d2._year,&d2._month,&d2._day);
  if(d1.IsInvalid() == false || d2.IsInvalid() == false)
  {
      printf("<html><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"><h2>您输入的日期格式有误，请重新输入</h2></html>");
  }
  else
  {
      int result = d1 - d2;
      printf("<html><h1>result=%d</h1></html>",result);
  }
  //printf("%d\n",result);
  return 0;
}

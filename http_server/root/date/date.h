#pragma once 


class Date
{
public:
  Date(int year = 1979,int month = 1,int day = 1)
    :_year(year)
     ,_month(month)
     ,_day(day)
  {}
  ~Date()
  {

  }
  Date(const Date& date)
  {
    _year = date._year;
    _month = date._month;
    _day = date._day;
  }
  Date& operator=(const Date& date)
  {
    if(this == &date)
    {
      return *this;
    }
    _year = date._year;
    _month = date._month;
    _day = date._day;
    return *this;
  }
  int GetCorrentDay(int year,int month);
  bool IsInvalid();
  bool IsLeapYear(int year);
  bool operator<(const Date& d);
  bool operator==(const Date& d);
  bool operator!=(const Date& d);
  Date& operator++();
  Date operator+(const int day);
  Date& operator+=(const int day);
  Date operator-(const int day);
  Date& operator-=(const int day);
  int operator-(Date& d);
  void PrintDate();
//private:
  int _year;
  int _month;
  int _day;
};

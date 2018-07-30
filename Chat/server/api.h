#pragma once
#include <string>
#include <jsoncpp/json/json.h>

//jsoncpp:用来解析json的C++的库.
namespace server
{
//data:可看做是服务器给客户端提供的api.
struct Data
{
    std::string name;
    std::string school;
    std::string msg;
    //command:quit表示退出
    std::string command;

    //序列化:把对象中的成员拼接为变成字符串
    void Serialize(std::string* output)
    {
        //json中的value类:可理解为unordered map来使用,保存数据
        Json::Value v;
        //可以理解为一个unordered_map
        v["name"] = name;
        v["school"] = school;
        v["msg"] = msg;
        v["command"] = command;
        Json::FastWriter w;  //这个就是用来序列化的对象
        *output = w.write(v);
    }
    //反序列化:把字符串填充到变成对象
    void UnSerialize(const std::string& input)
    {
        Json::Value v;
        Json::Reader r; //reader用来反序列化
        r.parse(input,v);
        //对于name等可以先判断一下json中存储的对象是否符合要求,合格后再进行赋值填充
        name = v["name"].asString(); //取出对象并转为字符串
        school = v["school"].asString();
        msg = v["msg"].asString();
        command = v["command"].asString();
    }
};

}

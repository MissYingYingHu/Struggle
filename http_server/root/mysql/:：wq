#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main()
{
  //使用MySQL api 完成查找动作
  //1.先根据CGI的规则，获取用户的相关参数，但是
  //由于此处只是把表中的所有数据都查出来，就不管用户的输入
  
  //初始化
  MYSQL* conn = mysql_init(NULL);
  //建立连接
  if(mysql_real_create(conn,"127.0.0.1","root","","Table",3306,NULL,0) == NULL)
  {
    fprintf(stderr,"mysql conn failed\n");
    return 1;
  }
  fprintf(stderr,"mysql conn OK\n");
  //构造SQL语句，select语句，就是字符串拼接
  const char* sql = "select * from Table";
  //把SQL语句发送到服务器之中
  int ret = mysql_query(conn,sql);
  if(ret < 0)
  {
    fprintf(stderr,"mysql_quary failed\n");
    return 1;
  }
  //遍历表结构，并打印
  MYSQL* res = mysql_store_result(conn);
  if(res == NULL)
  {
    fprintf(stderr,"mysql_store failed\n");
    return 1;
  }
  //先获取行列，然后获取表头字段，然后取出表内容
  int rows = mysql_num_rows(res);
  int fie = mysql_num_fields(res);
  MYSQL_FIELD* f = mysql_fetch_field(res);
  while(f)
  {
    //name是列名
    prinf("%s\t",f->name);
    f = mysql_fetch_field(res);
  }
  printf("<br>");

  printf("</html>");
  //断开连接
  return 0;
}

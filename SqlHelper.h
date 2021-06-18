#ifndef SQLHELPER_H
#define SQLHELPER_H
//20210521 gxx
//sql type {mysql}
#include "common.h"
#include "commonfunction_c.h"
#include <mysql.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using namespace commonfunction_c;
using namespace std;


namespace extensionfunction_c {
	class SqlHelperBase {
	public:
		SqlHelperBase() {}
		~SqlHelperBase(){}
		virtual bool connect(string connstr) = 0;
		virtual void free() = 0;
		virtual int* query(string queryString) = 0;
		virtual bool insert(string insertString) = 0;
		virtual bool del(string deleteString) = 0;
	};

	class MysqlHelper : public SqlHelperBase {
	private:
		MYSQL mysql; //mysql连接
		MYSQL_FIELD* fd = NULL;  //字段列数组
		char field[32][32] = { 0 };  //存字段名二维数组
		MYSQL_RES* res; //这个结构代表返回行的一个查询结果集
		char queryStr[150]; //查询语句
	public:
		MysqlHelper() {

		}
		bool connect(string config) {
			//解析config文件内的json数据，获取数据库配置
			boost::property_tree::ptree root;
			boost::property_tree::read_json(config, root);
			string host = root.get<string>("host");
			string user = root.get<string>("user");
			string passwd = root.get<string>("passwd");
			string database = root.get<string>("database");
			const int port = root.get<int>("port");
			//初始化mysql  
			mysql_init(&mysql);  //连接mysql，数据库  
			//返回false则连接失败，返回true则连接成功  
			if (!(mysql_real_connect(&mysql, host.c_str(), user.c_str(), passwd.c_str(), database.c_str(), port, NULL, 0)))
				//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
			{
				printf("Error connecting to database:%s\n", mysql_error(&mysql));
				return false;
			}
			else
			{
				printf("Connected...\n");
				return true;
			}
			return true;
		}

		void free() {
			mysql_free_result(res);  //释放一个结果集合使用的内存。
			mysql_close(&mysql);	 //关闭一个服务器连接。
		}

		int* query(string queryString) {
			mysql_query(&mysql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
				//返回0 查询成功，返回1查询失败  
			if (mysql_query(&mysql, queryString.c_str())) {        // 执行指定为一个空结尾的字符串的SQL查询。	
				printf("Query failed (%s)\n", mysql_error(&mysql));
				return NULL;
			}
			else {
				printf("query success\n");
			}
			//获取结果集  
			if (!(res = mysql_store_result(&mysql)))    //获得sql语句结束后返回的结果集  
			{
				printf("Couldn't get result from %s\n", mysql_error(&mysql));
				return NULL;
			}

			
			return (int*)(this->res);
		}

		bool insert(string insertString) {
			mysql_query(&mysql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
			// 返回执行结果，0表示插入成功返回true，否则返回false。
			return (mysql_query(&mysql, insertString.c_str()) == 0);   	
		}

		bool del(string deleteString) {
			mysql_query(&mysql, "set names gbk"); //设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码  
			// 返回执行结果，0表示插入成功返回true，否则返回false。 
			return (mysql_query(&mysql, deleteString.c_str()) == 0);  
		}
	};
}
#endif //SQLHELPER_H
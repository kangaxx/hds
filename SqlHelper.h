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
		MYSQL mysql; //mysql����
		MYSQL_FIELD* fd = NULL;  //�ֶ�������
		char field[32][32] = { 0 };  //���ֶ�����ά����
		MYSQL_RES* res; //����ṹ�������е�һ����ѯ�����
		char queryStr[150]; //��ѯ���
	public:
		MysqlHelper() {

		}
		bool connect(string config) {
			//����config�ļ��ڵ�json���ݣ���ȡ���ݿ�����
			boost::property_tree::ptree root;
			boost::property_tree::read_json(config, root);
			string host = root.get<string>("host");
			string user = root.get<string>("user");
			string passwd = root.get<string>("passwd");
			string database = root.get<string>("database");
			const int port = root.get<int>("port");
			//��ʼ��mysql  
			mysql_init(&mysql);  //����mysql�����ݿ�  
			//����false������ʧ�ܣ�����true�����ӳɹ�  
			if (!(mysql_real_connect(&mysql, host.c_str(), user.c_str(), passwd.c_str(), database.c_str(), port, NULL, 0)))
				//�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
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
			mysql_free_result(res);  //�ͷ�һ���������ʹ�õ��ڴ档
			mysql_close(&mysql);	 //�ر�һ�����������ӡ�
		}

		int* query(string queryString) {
			mysql_query(&mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
				//����0 ��ѯ�ɹ�������1��ѯʧ��  
			if (mysql_query(&mysql, queryString.c_str())) {        // ִ��ָ��Ϊһ���ս�β���ַ�����SQL��ѯ��	
				printf("Query failed (%s)\n", mysql_error(&mysql));
				return NULL;
			}
			else {
				printf("query success\n");
			}
			//��ȡ�����  
			if (!(res = mysql_store_result(&mysql)))    //���sql�������󷵻صĽ����  
			{
				printf("Couldn't get result from %s\n", mysql_error(&mysql));
				return NULL;
			}

			
			return (int*)(this->res);
		}

		bool insert(string insertString) {
			mysql_query(&mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
			// ����ִ�н����0��ʾ����ɹ�����true�����򷵻�false��
			return (mysql_query(&mysql, insertString.c_str()) == 0);   	
		}

		bool del(string deleteString) {
			mysql_query(&mysql, "set names gbk"); //���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������  
			// ����ִ�н����0��ʾ����ɹ�����true�����򷵻�false�� 
			return (mysql_query(&mysql, deleteString.c_str()) == 0);  
		}
	};
}
#endif //SQLHELPER_H
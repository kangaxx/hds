#pragma once
//20210520 by gxx
//��־����ϵͳ��Ŀǰ�ʺ�vc2019
#include <iostream>
#include <time.h>
#include <fstream>
#include <errors.h>
#include <direct.h>
#include <stdio.h>
#include <io.h>

#include"common.h"
#include "commonfunction_c.h"
namespace commonfunction_c {
	class Logger {
	public:
		//·������m_out ���ļ�ϵͳ·��������������豸·���ȣ�
		Logger(std::string o) : m_out(o){
		}

		void Initial(std::string f) {
			//���ø�ʽ
			m_format = f;
		}
		void Log(std::string l) {
			try {
				//������־����
				std::string log;
				if (m_format.empty()) {
					//���û�����ø�ʽ��Ĭ�Ͼ��� [������ ʱ����] : ƴ�� log 
					getDateTime(log);
					log = "[" + log + " ] LEVEL [Log] : " + l + "\n";
				}
				//�����form
				// to do 
				//�����console
				std::cout << log << std::endl;
				//������ļ�
				write2File(log);
			} catch (...) {
				// do nothing
			}
		}
	private:


		

		void getDateTime(string& strDate) {
			char chTmp[20] = { '\0' };
			time_t t;
			time(&t);
			struct tm p;
			localtime_s(&p, &t);
			p.tm_year = p.tm_year + 1900;
			p.tm_mon = p.tm_mon + 1;
			snprintf(chTmp, sizeof(chTmp), "%04d-%02d-%02d %2d:%02d:%04d",
				p.tm_year, p.tm_mon, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec);
			strDate = chTmp;
			return;
		}

		void getDate(string& strDate) {
			char chTmp[15] = { '\0' };
			time_t t;
			time(&t);
			struct tm p;
			localtime_s(&p, &t);
			p.tm_year = p.tm_year + 1900;
			p.tm_mon = p.tm_mon + 1;
			snprintf(chTmp, sizeof(chTmp), "%04d-%02d-%02d",
				p.tm_year, p.tm_mon, p.tm_mday);
			strDate = chTmp;
			return;
		}

		void write2File(string log) {
			assert(!m_out.empty());
			assert(BaseFunctions::isFolderExist(m_out.c_str()));
			string fileName;
			getDate(fileName);
			fileName = fileName + ".log";
			fileName = BaseFunctions::combineFilePath(m_out, fileName);
			FILE *fp = NULL;
			errno_t result;
			fopen_s(&fp, fileName.c_str(), "a+");
			if (fp != NULL) {
				fputs(log.c_str(), fp);
				fclose(fp);
			}
			return;
		}

		//out�ļ�����ʽ���ã� �滻����
		//%date �õ�ǰ��������� ��ʽ��2021-05-21����
		//%time �õ�ǰʱ�����   ��ʽ��18:02:31���� 
		string getFileName() {
			string result = "";
			return result;
		}
		//�����
		std::string m_out;
		std::string m_format = "";
	};
}
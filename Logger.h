#pragma once
//20210520 by gxx
//日志功能系统，目前适合vc2019
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
 		Logger() : m_out(DEFAULT_LOG_PATH_WIN) {

		}
		//路径设置m_out （文件系统路径或者其他输出设备路径等）
		Logger(std::string o) : m_out(o){
			log_level_ = LOG_LEVEL_STD_OUT | LOG_LEVEL_STD_WRITE_FILE;
		}

		void set_level(int level) {
			log_level_ = level;
		}

		void Initial(std::string f) {
			//设置格式
			m_format = f;
		}
		void Log(std::string l) {
			try {
				//整理日志内容
				std::string log;
				if (m_format.empty()) {
					//如果没有设置格式，默认就是 [年月日 时分秒] : 拼接 log 
					getDateTime(log);
					log = "[" + log + " ] LEVEL [Log] : " + l + "\n";
				}
				//输出到form
				// to do 
				//输出到console
				if (log_level_ & LOG_LEVEL_STD_OUT)
					std::cout << log << std::endl;
				//输出到文件
				if (log_level_ & LOG_LEVEL_STD_WRITE_FILE)
					write2File(log);
			} catch (...) {
				// do nothing
			}
		}

		void Log(std::wstring l) {
			Log(BaseFunctions::ws2s(l));
		}

		void SimpleLog(std::string fileName, std::string l) {
			try {
				//整理日志内容
				std::string log;
				if (m_format.empty()) {
					//如果没有设置格式，默认就是 [年月日 时分秒] : 拼接 log 
					log = l + "\n";
				}
				//输出到form
				// to do 
				//输出到console
				std::cout << log << std::endl;
				//输出到文件
				write2File(fileName, log);
			}
			catch (...) {
				// do nothing
			}
		}

		//可以持续追加的log方式 ,my_log.linkable_log("this").linkable_log(" is").Log(" Sparta!");
		Logger linkable_log(std::string l) {
			try {
				//整理日志内容
				std::string log;
				if (m_format.empty()) {
					//如果没有设置格式，默认就是 [年月日 时分秒] : 拼接 log 
					getDateTime(log);
					log = "[" + log + " ] LEVEL [Log] : " + l;
				}
				//输出到form
				// to do 
				//输出到console
				if (log_level_ & LOG_LEVEL_STD_OUT)
					std::cout << log;
				//输出到文件
				if (log_level_ & LOG_LEVEL_STD_WRITE_FILE)
					write2File(log);
				return *this;
			}
			catch (...) {
				// do nothing
			}
		}

		Logger linkable_log(std::wstring l) {
			return linkable_log(BaseFunctions::ws2s(l));
		}

		//清空log文件
		void clear_log_file() {

		}
	private:
		int log_level_; //1;默认输出 2:写入日志文件 4: 写入数据库
		void getDateTime(string& strDate) {
			char chTmp[24] = { '\0' };
			time_t t;
			time(&t);
			struct tm p;
			localtime_s(&p, &t);
			p.tm_year = p.tm_year + 1900;
			p.tm_mon = p.tm_mon + 1;
			snprintf(chTmp, sizeof(chTmp), "%04d-%02d-%02d %2d:%02d:%02d",
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
			string fileName;
			getDate(fileName);
			fileName = fileName + ".log";
			return write2File(fileName, log);
		}

		void write2File(string fileName, string log) {
			assert(!m_out.empty());
			BaseFunctions::createDirectoryW(BaseFunctions::s2ws(m_out));
			fileName = fileName;
			fileName = BaseFunctions::combineFilePath(m_out, fileName);
			FILE* fp = NULL;
			errno_t result;
			fopen_s(&fp, fileName.c_str(), "a+");
			if (fp != NULL) {
				fputs(log.c_str(), fp);
				fclose(fp);
			}
			return;
		}
		//out文件名格式设置， 替换规则
		//%date 用当前日期替代， 格式是2021-05-21这样
		//%time 用当前时间替代   格式是18:02:31这样 
		string getFileName() {
			string result = "";
			return result;
		}
		//输出流
		std::string m_out;
		std::string m_format = "";
	};
}
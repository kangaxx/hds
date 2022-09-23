//20220905 gxx Taize corp.
/*****************************************************************************************

�߱������̵߳���־Logger
����ģʽ + �۲����Ѷ���
�����������г�ʼ�������ⲻͬ�ڴ�ռ�����ֶ��ʵ��������־˳��
�̳��˻�����log�࣬�������Դ��ݸ���̬���ӿ����lib��ʵ����־���ܡ�

*****************************************************************************************/
#pragma once
#include <iostream>
#include <redis++.h>
#include <cstdio>
#include <unordered_set>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <UserEnv.h>

#include "commonfunction_c.h"
#include "common.h"
#include "Logger.h"
#include "../../../hds/halconUtils.h"
namespace commonfunction_c {
	class ConcurrentLogger :public LoggerBase {
	public:
		void append_log(const char* log);
		void set_level(int level) {
		}
		void Log(std::string l);
		static ConcurrentLogger* get_instance(const char* path);
		void loop_cache_thread();
	private:	
		std::mutex _read_write_cache_mutex;
		size_t _log_length = 200;
		class LogUnit {
		public:
			LogUnit(size_t text_length, const char* log) {
				_length = text_length;
				_log = new char[_length];
				strncpy_s(_log, _length, log, _length - 1);
			}
			~LogUnit() {
				delete[] _log;
			}
			const char* get_log() const;
			void copy_log(char* destination) const;
		private:
			size_t _length;
			char* _log;
		};
		commonfunction_c::DuLink<LogUnit*> _log_list;
		char _path[THREAD_SAFE_LOGGER_PATH_CHAR_LENGTH] = { 0 };
		Logger* _logger;
		ConcurrentLogger() {
			//nothing
			throw "Do not use this function";
		}
		ConcurrentLogger(const char* path) {
			strncpy_s(_path, path, THREAD_SAFE_LOGGER_PATH_CHAR_LENGTH);
			_logger = new Logger(_path);;
		}
		bool initial(const char* v);
	};
}
//20220905 gxx Taize corp.
/*****************************************************************************************

具备独立线程的日志Logger
单例模式 + 观察消费队列
请在主程序中初始化，避免不同内存空间里出现多个实例搞乱日志顺序
继承了基本的log类，这样可以传递给动态链接库或者lib，实现日志功能。

*****************************************************************************************/
#pragma once
#include <iostream>

#include "commonfunction_c.h"
#include "common.h"
#include "Logger.h"

namespace commonfunction_c {
	unsigned long thread_safe_log_start_thread(void* lpParameter);
	void* g_singleton_thread_safe_logger = NULL;
	std::mutex g_singleton_mutex;

	class ConcurrentLogger :public LoggerBase {
	public:
		void append_log(char* log) {
		}
		void set_level(int level) {
		}
		void Log(std::string l) {
		}
		static ConcurrentLogger* get_instance(char* path) {
			if (NULL == g_singleton_thread_safe_logger) {
				g_singleton_mutex.lock();
				if (NULL == g_singleton_thread_safe_logger) {
					ConcurrentLogger* temp_instance = new ConcurrentLogger(path);
					temp_instance->initial(NULL);
					g_singleton_thread_safe_logger = temp_instance;
				}
				g_singleton_mutex.unlock();
			}
			return (ConcurrentLogger*)g_singleton_thread_safe_logger;
		}

		bool initial(char* v) {
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&thread_safe_log_start_thread, NULL, 0, 0);
			return true;
		}

		void log(char* log) {
			g_singleton_mutex.lock();
			g_singleton_mutex.unlock();
		}

		bool write_log(int idx) {
			if (idx < get_log_count()) {
				//_logger->Log(_logs.at(idx));
				return true;
			}
			else
				return false;

		}
		bool is_new_log(int idx) {
			return idx < get_log_count();
		}

	private:	
		class LogUnit {
		public:
			LogUnit(char* p) {
				_p = p;
			}
		private:
			char* _p;
		};
		char _path[THREAD_SAFE_LOGGER_PATH_CHAR_LENGTH] = { 0 };
		int _log_count = 0;
		const int LOG_LENGTH = CONCURRENT_LOGGER_DATA_CACHE_SIZE / CONCURRENT_LOGGER_COUNT_MAX;
		char _logs[CONCURRENT_LOGGER_DATA_CACHE_SIZE];
		Logger* _logger;
		ConcurrentLogger() {
			//nothing
			throw "Do not use this function";
		}
		ConcurrentLogger(char* path) {
			strncpy_s(_path, path, THREAD_SAFE_LOGGER_PATH_CHAR_LENGTH);
			_logger = new Logger(_path);;
		}

		int get_log_count() { return _log_count; }
		void inc_log_count() { _log_count++; }
	};

	unsigned long thread_safe_log_start_thread(void* lpParameter) {
		int idx = 0;
		while (true) {
			bool is_new_log;
			g_singleton_mutex.lock();
			is_new_log = ((ConcurrentLogger*)g_singleton_thread_safe_logger)->is_new_log(idx);
			g_singleton_mutex.unlock();
			if (is_new_log && ((ConcurrentLogger*)g_singleton_thread_safe_logger)->write_log(idx))
				idx++;
			else
				Sleep(5);
		}
	}
}
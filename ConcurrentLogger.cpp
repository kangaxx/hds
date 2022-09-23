#include "ConcurrentLogger.h"
static std::mutex _singleton_mutex;
void* g_singleton_thread_safe_logger = NULL;
using namespace commonfunction_c;

ConcurrentLogger* commonfunction_c::ConcurrentLogger::get_instance(const char* path)
{
	if (NULL == g_singleton_thread_safe_logger) {
		_singleton_mutex.lock();
		if (NULL == g_singleton_thread_safe_logger) {
			ConcurrentLogger* temp_instance = new ConcurrentLogger(path);
			temp_instance->initial(NULL);
			g_singleton_thread_safe_logger = temp_instance;
		}
		_singleton_mutex.unlock();
	}
	return (ConcurrentLogger*)g_singleton_thread_safe_logger;
}

bool commonfunction_c::ConcurrentLogger::initial(const char* v)
{
	std::thread loop_cache_thread(&ConcurrentLogger::loop_cache_thread, this);
	loop_cache_thread.detach();
	return true;
}

void commonfunction_c::ConcurrentLogger::Log(string log) {
	append_log(log.c_str());
}

void commonfunction_c::ConcurrentLogger::append_log(const char* log)
{
	LogUnit* log_node = new LogUnit(_log_length, log);
	_read_write_cache_mutex.lock();
	_log_list.addToLast(log_node);
	_read_write_cache_mutex.unlock();
}

void commonfunction_c::ConcurrentLogger::loop_cache_thread()
{
	while (true) {
		bool is_get_log = false;
		char* temp_log = new char[_log_length];
		_read_write_cache_mutex.lock();
		if (!_log_list.isEmpty()) {
			_log_list.getElement(0)->copy_log(temp_log);
			_log_list.del(0);
			is_get_log = true;
		}
		_read_write_cache_mutex.unlock();
		if (is_get_log)
			_logger->Log(temp_log);
		Sleep(1000);
	}
}

const char* commonfunction_c::ConcurrentLogger::LogUnit::get_log() const
{
	return _log;
}

void commonfunction_c::ConcurrentLogger::LogUnit::copy_log(char* destination) const
{
	strncpy_s(destination, _length, _log, _length);
}

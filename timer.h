#ifndef TIMER_H_INCLUDE
#define TIMER_H_INCLUDE
#include <sys/timeb.h>
namespace commonfunction_c {
	enum class COMMON_TIMER_STATE { COMMON_TIME_STATE_RUNNING = 1, COMMON_TIME_STATE_STOP = 2, COMMON_TIME_STATE_PAUSE = 9 } time_state;
	class timer {
	public:
		static timer& getInstance() { return _instance; }
		void add(int v = 1) { 
			if (_state == COMMON_TIMER_STATE::COMMON_TIME_STATE_RUNNING) {
				_count += v;
				updateActionTime();
			}
		}

		void start() {
			//后续添加一个自动执行计时并更新系统时间记录参数

			if (_state != COMMON_TIMER_STATE::COMMON_TIME_STATE_RUNNING) {
				_state = COMMON_TIMER_STATE::COMMON_TIME_STATE_RUNNING;
				updateStartTime();
			}
			return;
		}
		
		void stop() {
			if (_state == COMMON_TIMER_STATE::COMMON_TIME_STATE_RUNNING) {
				clear();
			}
		}

		void clear() {
			_count = 0;
			_state = COMMON_TIMER_STATE::COMMON_TIME_STATE_STOP;
			_startTime = -1;
			_actionTime = -1;
			_stopTime = -1;
		}

		long getCount() { return _count; }
		time_t getWorkingTime() { 
			long long result = getWorkingTimeMillisec();
			return result <= 1 ? result : result / 1000;
		}

		time_t getWorkingTimeMillisec() {
			if (_state == COMMON_TIMER_STATE::COMMON_TIME_STATE_RUNNING && _actionTime >= 0 && _startTime >= 0)
				return (_actionTime - _startTime);
			else
				return -1;
		}

		time_t getActionTime() {
			return _actionTime;
		}

		std::string getActionTimeString() {
			std::stringstream result;
			result << _actionTime;
			return result.str();
		}

		std::string getWorkTimeString() {
			std::stringstream result;
			result << getWorkingTimeMillisec();
			return result.str();
		}
	private:
		static timer _instance;
		long _count;
		time_t _startTime = -1;
		time_t _actionTime = -1;
		time_t _stopTime = -1;
		COMMON_TIMER_STATE _state;
		timer() {
			_count = 0;
			_state = COMMON_TIMER_STATE::COMMON_TIME_STATE_STOP;
		}
		

		timer(const timer& right) {
			_count = right._count;
			_state = right._state;
			_startTime = right._startTime;
			_actionTime = right._actionTime;
			_stopTime = right._stopTime;
		}

		void operator=(const timer& right) {
			_count = right._count;
			_state = right._state;
			_startTime = right._startTime;
			_actionTime = right._actionTime;
			_stopTime = right._stopTime;
		}

		void getTime(time_t &out){ 
			timeb t;
			ftime(&t);
			out = t.millitm + t.time * 1000;
		}

		void updateStartTime() {
			getTime(_startTime);
		}

		void updateActionTime() {
			getTime(_actionTime);
		}

		void updateStopTime() {
			getTime(_stopTime);
		}
	};
	timer timer::_instance;
}

#endif TIMER_H_INCLUDE

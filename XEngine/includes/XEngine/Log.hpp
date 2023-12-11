#ifndef LOG_H
#define LOG_H

#include <sstream>

namespace XEngine {

#ifdef NDEBUG
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERRR(...)
#define LOG_CRIT(...)
#else
#define LOG_INFO(...) Log::log_info(__VA_ARGS__);
#define LOG_WARN(...) Log::log_warn(__VA_ARGS__);
#define LOG_ERRR(...) Log::log_error(__VA_ARGS__);
#define LOG_CRIT(...) Log::log_critical(__VA_ARGS__);
	class Log {
	public:
		static void log_info(const char* t_msg) { log(t_msg, "INFO", 7); }
		static void log_warn(const char* t_msg) { log(t_msg, "WARN", 6); }
		static void log_error(const char* t_msg) { log(t_msg, "ERRR", 4); }
		static void log_critical(const char* t_msg) { log(t_msg, "CRIT", 13); }
	private:
		static void log(const char* t_msg, const char* t_type, const int t_mod);
	};
#endif

}

#endif // LOG_H
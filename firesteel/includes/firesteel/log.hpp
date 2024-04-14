#ifndef LOG_H
#define LOG_H

#include <string>
#include <sstream>

namespace firesteel {

#define LOG(...) Log::log_ntag(__VA_ARGS__);
#define LOG_INFO(...) Log::log_info(__VA_ARGS__);
#define LOG_WARN(...) Log::log_warn(__VA_ARGS__);
#define LOG_ERRR(...) Log::log_error(__VA_ARGS__);
#define LOG_CRIT(...) Log::log_critical(__VA_ARGS__);
	class Log {
	public:
#ifndef NDEBUG
		static void log_ntag(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5, "", 15); }
		static void log_info(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5, "[INFO] ", 7); }
		static void log_warn(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5, "[WARN] ", 6); }
		static void log_error(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5, "[ERRR] ", 4); }
		static void log_critical(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5, "[CRIT] ", 13); }
	private:
		static void log(std::string t_msg, std::string t_type, const int t_mod);
#else
		static void log_ntag(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { }
		static void log_info(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { }
		static void log_warn(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { }
		static void log_error(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { }
		static void log_critical(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") { }
#endif
	};

}

#endif // LOG_H
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
#ifdef WIN32
#define CMD_F_BLACK 0
#define CMD_F_BLUE 1
#define CMD_F_GREEN 2
#define CMD_F_CYAN 3
#define CMD_F_RED 4
#define CMD_F_PURPLE 5
#define CMD_F_YELLOW 6
#define CMD_F_GRAY 7
#define CMD_F_LBLACK 8
#define CMD_F_LBLUE 9
#define CMD_F_LGREEN 10
#define CMD_F_LCYAN 11
#define CMD_F_LRED 12
#define CMD_F_LPURPLE 13
#define CMD_F_LYELLOW 14
#define CMD_F_WHITE 15

#define CMD_BG_BLACK 0
#define CMD_BG_BLUE 16
#define CMD_BG_GREEN 32
#define CMD_BG_CYAN 48
#define CMD_BG_RED 64
#define CMD_BG_PURPLE 80
#define CMD_BG_YELLOW 96
#define CMD_BG_GRAY 112
#define CMD_BG_LBLACK 128
#define CMD_BG_LBLUE 144
#define CMD_BG_LGREEN 160
#define CMD_BG_LCYAN 176
#define CMD_BG_LRED 192
#define CMD_BG_LPURPLE 208
#define CMD_BG_LYELLOW 224
#define CMD_BG_WHITE 240

#endif
#ifndef NDEBUG
		static void log_ntag(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") {
			log(std::string("") + t_msg1 + t_msg2 + t_msg3 + t_msg4 + t_msg5 + "\n");
		}
		static void log_info(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") {
			log("[INFO] ", CMD_F_GRAY);
			log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5 + "\n");
		}
		static void log_warn(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") {
			log("[INFO] ", CMD_F_YELLOW);
			log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5 + "\n");
		}
		static void log_error(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") {
			log("[INFO] ", CMD_F_RED);
			log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5 + "\n");
		}
		static void log_critical(const char* t_msg1, const char* t_msg2 = "", const char* t_msg3 = "", const char* t_msg4 = "", const char* t_msg5 = "") {
			log("[INFO] ", CMD_BG_RED + CMD_F_WHITE);
			log(std::string(t_msg1) + t_msg2 + t_msg3 + t_msg4 + t_msg5 + "\n");
		}
		/// <summary>
		/// Logs message to console with specified color and prompt.
		/// </summary>
		/// <param name="t_msg">Text of message.</param>
		/// <param name="t_mod">Color of text (+ bg, if supported)</param>
		static void log(std::string t_msg, const int t_mod = CMD_F_WHITE);
		static void clear() {
#ifdef WIN32
			system("cls");
#endif // WIN32
		}
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
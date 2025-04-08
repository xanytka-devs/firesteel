#ifndef FS_LOG_H
#define FS_LOG_H

#include <sstream>
#include <string>
#include <iostream>

#define LOG_C(...) Log::log_c(__VA_ARGS__);
#define LOG(...) Log::log_ntag(__VA_ARGS__);
#define LOG_INFO(...) Log::log_info(__VA_ARGS__);
#define LOG_STATE(...) Log::log_state(__VA_ARGS__);
#define LOG_WARN(...) Log::log_warn(__VA_ARGS__);
#define LOG_ERRR(...) Log::log_error(__VA_ARGS__);
#define LOG_CRIT(...) Log::log_critical(__VA_ARGS__);
	class Log {
	public:
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
	static void log_c(const std::string& tMsg, const int t_mod = CMD_F_WHITE, const char* tEndLine = "\n") {
		log(tMsg + tEndLine, t_mod);
	}
	static void log_ntag(const std::string& tMsg, const char* tEndLine = "\n") {
		log(tMsg + tEndLine);
	}
	static void log_info(const std::string& tMsg, const char* tEndLine = "\n") {
		log("[INFO] ", CMD_F_GRAY);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void log_state(const std::string& tMsg, const char* tEndLine = "\n") {
		log("[STATE] ", CMD_F_LBLUE);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void log_warn(const std::string& tMsg, const char* tEndLine = "\n") {
		log("[WARN] ", CMD_F_YELLOW);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void log_error(const std::string& tMsg, const char* tEndLine = "\n") {
		log("[ERRR] ", CMD_F_RED);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void log_critical(const std::string& tMsg, const char* tEndLine = "\n") {
		log("[CRIT]", CMD_BG_RED + CMD_F_WHITE);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	/// Logs message to console with specified color and prompt.
	static void log(const std::string& tMsg, const int t_mod = CMD_F_WHITE, const bool tAddTimestamp = true);
	// Clears console log window.
	static void clear();
	// Logs something only to file.
	static void logToFile(const char* tMsg, const bool tAddTimestamp);
	// [DANGEROUS]
	// Can cause deletion of 'latest.log'.
	static void destroyFileLogger();
};

#endif // !FS_LOG_H
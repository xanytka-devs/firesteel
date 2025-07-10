#ifndef FS_LOG_H
#define FS_LOG_H

#include <sstream>
#include <string>
#include <iostream>

#define LOG_C(...) Log::logCustom(__VA_ARGS__);
#define LOG(...) Log::logNoTag(__VA_ARGS__);
#define LOG_INFO(...) Log::logInfo(__VA_ARGS__);
#define LOG_STATE(...) Log::logState(__VA_ARGS__);
#define LOG_WARN(...) Log::logWarning(__VA_ARGS__);
#define LOG_ERRR(...) Log::logError(__VA_ARGS__);
#define LOG_ERROR(...) Log::logError(__VA_ARGS__);
#define LOG_CRIT(...) Log::logCritical(__VA_ARGS__);
#ifdef FS_PRINT_DEBUG_MSGS
//[!WARNING]
//This function is limited to framework-specific debug messages and won't work if 'FS_PRINT_DEBUG_MSGS' isn't defined.
//Please use LOG_C(...) instead.
#define LOG_DEBUG(...) Log::logDebug(__VA_ARGS__);
//[!WARNING]
//This function is limited to framework-specific debug messages and won't work if 'FS_PRINT_DEBUG_MSGS' isn't defined.
//Please use LOG_C(...) instead.
#define LOG_DBG(...) Log::logDebug(__VA_ARGS__);
#endif // FS_PRINT_DEBUG_MSGS

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
	class Log {
	public:
	static void logCustom(const std::string& tMsg, const int tMod=CMD_F_WHITE, const char* tEndLine="\n") {
		log(tMsg + tEndLine, tMod);
	}
	static void logNoTag(const std::string& tMsg, const char* tEndLine="\n") {
		log(tMsg + tEndLine);
	}
	static void logInfo(const std::string& tMsg, const char* tEndLine="\n") {
		log("[INFO] ", CMD_F_GRAY);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void logState(const std::string& tMsg, const char* tEndLine="\n") {
		log("[STATE] ", CMD_F_LBLUE);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void logWarning(const std::string& tMsg, const char* tEndLine="\n") {
		log("[WARN] ", CMD_F_YELLOW);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void logError(const std::string& tMsg, const char* tEndLine="\n") {
		log("[ERRR] ", CMD_F_RED);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
	static void logCritical(const std::string& tMsg, const char* tEndLine="\n") {
		log("[CRIT] ", CMD_BG_RED + CMD_F_WHITE);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
#ifdef FS_PRINT_DEBUG_MSGS
	//[!WARNING]
	//This function is limited to framework-specific debug messages and won't work if 'FS_PRINT_DEBUG_MSGS' isn't defined.
	//Please use LOG_C(...) instead.
	static void logDebug(const std::string& tMsg, const char* tEndLine="\n") {
		log("[DBG] ", CMD_F_PURPLE);
		log(tMsg + tEndLine, CMD_F_WHITE, false);
	}
#endif // FS_PRINT_DEBUG_MSGS
	/// Logs message to console with specified color and prompt.
	static void log(const std::string& tMsg, const int tMod=CMD_F_WHITE, const bool tAddTimestamp=true);
	// Clears console log window.
	static void clear();

	// Allows/disallows file logging all together. Depends on global cfg file.
	static bool sSaveLogs;
	// Logs something only to file.
	static void logToFile(const char* tMsg, const bool tAddTimestamp);
	// [DANGEROUS]
	// Can cause deletion of 'latest.log'.
	static void destroyFileLogger();
};

#endif // !FS_LOG_H
#ifndef FS_LOG_H
#define FS_LOG_H

#include <sstream>
#include <string>
#include <iostream>

#ifdef WIN32
/* FONT COLORS */

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
/* BACKGROUND COLORS*/

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
#endif // WIN32
#ifdef __linux__
/* FONT COLORS */

#define CMD_F_BLACK 30
#define CMD_F_BLUE 34
#define CMD_F_GREEN 32
#define CMD_F_CYAN 36
#define CMD_F_RED 31
#define CMD_F_PURPLE 35
#define CMD_F_YELLOW 33
#define CMD_F_GRAY 37
#define CMD_F_LBLACK 90
#define CMD_F_LBLUE 94
#define CMD_F_LGREEN 92
#define CMD_F_LCYAN 96
#define CMD_F_LRED 91
#define CMD_F_LPURPLE 95
#define CMD_F_LYELLOW 93
#define CMD_F_WHITE 97
/* BACKGROUND COLORS*/

#define CMD_BG_BLACK 40
#define CMD_BG_BLUE 44
#define CMD_BG_GREEN 42
#define CMD_BG_CYAN 46
#define CMD_BG_RED 41
#define CMD_BG_PURPLE 45
#define CMD_BG_YELLOW 43
#define CMD_BG_GRAY 47
#define CMD_BG_LBLACK 100
#define CMD_BG_LBLUE 104
#define CMD_BG_LGREEN 102
#define CMD_BG_LCYAN 106
#define CMD_BG_LRED 101
#define CMD_BG_LPURPLE 105
#define CMD_BG_LYELLOW 103
#define CMD_BG_WHITE 107
#endif // __linux__
/* SIMPLE FUNCTIONS */

#define TLOG(time, ...)		{Log::simple(time,__VA_ARGS__);}
#define LOG(...)			{TLOG(true,__VA_ARGS__);}
#define LOG_INFO(...)		{Log::log("[INFO] ",false,CMD_F_GRAY);TLOG(false,__VA_ARGS__);}
#define LOG_STATE(...)		{Log::log("[STAT] ",false,CMD_F_LBLUE);TLOG(false,__VA_ARGS__);}
#define LOG_STAT(...)		{LOG_STATE(__VA_ARGS__);}
#define LOG_WARNING(...)	{Log::log("[WARN] ",false,CMD_F_YELLOW);TLOG(false,__VA_ARGS__);}
#define LOG_WARN(...)		{LOG_WARNING(__VA_ARGS__);}
#define LOG_ERROR(...)		{Log::log("[ERRR] ",false,CMD_F_RED);TLOG(false,__VA_ARGS__);}
#define LOG_ERRR(...)		{LOG_ERROR(__VA_ARGS__);}
#define LOG_CRITICAL(...)	{Log::log("[CRIT] ",false,CMD_BG_RED+CMD_F_WHITE);TLOG(false,__VA_ARGS__);}
#define LOG_CRIT(...)		{LOG_CRITICAL(__VA_ARGS__);}
/* FORMATTED FUNCTIONS */

#define LOGF(format, ...)			{Log::log(Log::formatStr(format,__VA_ARGS__),true,CMD_F_WHITE,CMD_BG_BLACK,false);}
#define LOGF_C(format, fnt, bg, endl, ...){Log::log(Log::formatStr(format,__VA_ARGS__),endl,fnt,bg,false);}
#define LOGF_INFO(format, ...)		{Log::log("[INFO] ",false,CMD_F_GRAY);LOGF(format,__VA_ARGS__);}
#define LOGF_STATE(format, ...)		{Log::log("[STAT] ",false,CMD_F_LBLUE);LOGF(format,__VA_ARGS__);}
#define LOGF_STAT(format, ...)		{LOGF_STATE(format,__VA_ARGS__);}
#define LOGF_WARNING(format, ...)	{Log::log("[WARN] ",false,CMD_F_YELLOW);LOGF(format,__VA_ARGS__);}
#define LOGF_WARN(format, ...)		{LOGF_WARNING(format,__VA_ARGS__);}
#define LOGF_ERROR(format, ...)		{Log::log("[ERRR] ",false,CMD_F_RED);LOGF(format,__VA_ARGS__);}
#define LOGF_ERRR(format, ...)		{LOGF_ERROR(format,__VA_ARGS__);}
#define LOGF_CRITICAL(format, ...)	{Log::log("[CRIT] ",false,CMD_F_WHITE,CMD_BG_RED);LOGF(format,__VA_ARGS__);}
#define LOGF_CRIT(format, ...)		{LOGF_CRITICAL(format,__VA_ARGS__);}
#ifdef FS_PRINT_DEBUG_MSGS
// [!WARNING]
// This function is limited to framework-specific debug messages and won't work if 'FS_PRINT_DEBUG_MSGS' isn't defined.
// Please use LOG(...) or LOGF_C(format, color, ...) instead.
#define LOG_DEBUG(...) {Log::log("[DBUG] ",false,CMD_F_LGREEN);TLOG(false,__VA_ARGS__);}
// [!WARNING]
// This function is limited to framework-specific debug messages and won't work if 'FS_PRINT_DEBUG_MSGS' isn't defined.
// Please use LOG(...) or LOGF_C(format, color, ...) instead.
#define LOG_DBG(...) {LOG_DEBUG(__VA_ARGS__);}
// [!WARNING]
// This function is limited to framework-specific debug messages and won't work if 'FS_PRINT_DEBUG_MSGS' isn't defined.
// Please use LOGF(...) or LOGF_C(format, color, ...) instead.
#define LOGF_DEBUG(format, ...) {Log::log("[DBUG] ",false,CMD_F_LGREEN);LOGF(format,__VA_ARGS__);}
// [!WARNING]
// This function is limited to framework-specific debug messages and won't work if 'FS_PRINT_DEBUG_MSGS' isn't defined.
// Please use LOGF(...) or LOGF_C(format, color, ...) instead.
#define LOGF_DBG(format, ...) {LOGF_DEBUG(format,__VA_ARGS__);}
#endif // FS_PRINT_DEBUG_MSGS

	class Log {
	public:
	static void simple(const bool& tPlaceTime, const std::string& tMsg, const bool& tEndLine=true, const int tModF=CMD_F_WHITE, const int tModB=CMD_BG_BLACK) {
		log(tMsg, tEndLine, tModF, tModB, tPlaceTime);
	}
	static void log(const std::string& tMsg, const bool& tEndLine=true, const int tModF=CMD_F_WHITE, const int tModB=CMD_BG_BLACK, const bool tAddTimestamp=true);

	// Clears console log window. Does not affect the log file.
	static void clear();
	// Allows/disallows file logging all together. Depends on global cfg file.
	static bool sSaveLogs;
	// Logs something only to file.
	static void logToFile(const char* tMsg, const bool tAddTimestamp, const bool& tEndLine);
	// [!DANGEROUS]
	// Can cause deletion of 'latest.log'.
	static void destroyFileLogger();

	template<typename ... Args>
	static std::string formatStr(const std::string& format, Args ... args) {
		//Extra space for '\0'.
	    int sizeStr=std::snprintf(nullptr,0,format.c_str(),args ...)+1; 
	    if(sizeStr<=0) return "Invalid formatting string";
	    auto size=static_cast<size_t>(sizeStr);
		//Create buffer and fill it with formatted info.
	    std::unique_ptr<char[]> buf(new char[size]);
	    std::snprintf(buf.get(),size,format.c_str(),args ...);
		//Remove the '\0'.
	    return std::string(buf.get(),buf.get()+size-1);
	}
};

#endif // !FS_LOG_H
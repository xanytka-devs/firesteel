#include <firesteel/util/date_time.hpp>
#include <firesteel/util/log.hpp>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <time.h>

bool gInited = false;
std::ofstream gLogStream;

bool Log::sSaveLogs = true;
void Log::logToFile(const char* tMsg, const bool tAddTimestamp, const bool& tEndLine) {
	if (!gInited) {
		if (!std::filesystem::exists("logs/"))
			std::filesystem::create_directory("logs");
		gLogStream.open("logs/latest.log", std::ofstream::out | std::ofstream::trunc);
		gLogStream.close();
		gLogStream.open("logs/latest.log", std::ios::app);
		gInited = true;
	}
	std::ostringstream logEntry;
	if (tAddTimestamp) logEntry << DateTime::formatted("%X") + " ";
	logEntry << tMsg;
	if (tEndLine) logEntry << std::endl;
	gLogStream << logEntry.str();
	gLogStream.flush();
}
void Log::destroyFileLogger() {
	gLogStream.close();
	gInited = false;
	std::fstream src("logs/latest.log", std::ios::in | std::ios::binary);
	if (!src.good()) {
		LOG_ERRR("Couldn't open 'latest.log'");
		return;
	}
	std::string destFileName = "logs/" + DateTime::formatted("%d-%m-%Y %X") + ".log";
	std::replace(destFileName.begin(), destFileName.end(), ':', '.');
	std::ofstream dest(destFileName, std::ios::trunc | std::ios::binary);
	if (!dest.good()) {
		LOG_ERRR("Couldn't backup 'latest.log'");
		return;
	}
	dest << src.rdbuf();
	src.close();
	dest.close();
}

#ifdef WIN32
#include <windows.h>
void Log::log(const std::string& tMsg, const bool& tEndLine, const int tModF, const int tModB, const bool tAddTimestamp) {
	logToFile(tMsg.c_str(), tAddTimestamp, tEndLine);
#if !defined(NDEBUG) || defined(FS_HEADLESS)
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
	SetConsoleTitleA("Firesteel Debug Output");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, tModF + tModB);
	std::cout << tMsg;
	if (tEndLine) std::cout << std::endl;
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // !NDEBUG
}
void Log::clear() {
	system("cls");
}
#endif // WIN32
#ifdef __linux__
#include <cstdlib>

void Log::log(const std::string& tMsg, const bool& tEndLine, const int tModF, const int tModB, const bool tAddTimestamp) {
	logToFile(tMsg.c_str(), tAddTimestamp, tEndLine);
#ifndef NDEBUG or FS_HEADLESS
	std::cout << "\033]0;" << "Firesteel Debug Output" << "\007";
	std::string colorFormat = formatStr("\033[%i;%im", tModF, tModB);
	std::cout << colorFormat << tMsg;
	if (tEndLine) std::cout << std::endl;
#else
	system("ls -l > /dev/null 2>&1");
#endif // !NDEBUG
}
void Log::clear() {
	system("clear");
}
#endif // __linux__
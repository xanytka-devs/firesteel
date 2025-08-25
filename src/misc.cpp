#include "input/input.hpp"
using namespace Firesteel;

bool Keyboard::mKeys[GLFW_KEY_LAST]={ 0 };
bool Keyboard::mKeysChanged[GLFW_KEY_LAST]={ 0 };
bool Keyboard::mAnyKeyPressed=false;

float Mouse::mX=0;
float Mouse::mY=0;
float Mouse::m_old_x=0;
float Mouse::m_old_y=0;

float Mouse::mDX=0;
float Mouse::mDY=0;

float Mouse::mWheelDX=0;
float Mouse::mWheelDY=0;

bool Mouse::mFirstMove=true;
bool Mouse::mButtons[GLFW_MOUSE_BUTTON_LAST]={ 0 };
bool Mouse::mButtonsChanged[GLFW_MOUSE_BUTTON_LAST]={ 0 };



#include "utils/log.hpp"
#include <fstream>
#include <filesystem>

bool gInited=false;
std::ofstream gLogStream;

static const std::string currentDateTime(const char* tFormat) {
	struct tm newtime;
	__time64_t long_time;
	char timebuf[26];
	errno_t err;

	// Get time as 64-bit integer.
	_time64(&long_time);
	// Convert to local time.
	err=_localtime64_s(&newtime, &long_time);
	if (err) {
		LOG_WARN("Invalid argument to _localtime64_s.");
		return "invalid";
	}
	strftime(timebuf, sizeof(timebuf), tFormat, &newtime);
	return timebuf;
}

bool Log::sSaveLogs=true;
void Log::logToFile(const char* tMsg, const bool tAddTimestamp, const bool& tEndLine) {
	if (!gInited) {
		if(!std::filesystem::exists("logs/"))
			std::filesystem::create_directory("logs");
		gLogStream.open("logs/latest.log", std::ofstream::out | std::ofstream::trunc);
		gLogStream.close();
		gLogStream.open("logs/latest.log", std::ios::app);
		gInited=true;
	}
	std::ostringstream logEntry;
	if(tAddTimestamp) logEntry << currentDateTime("%X") + " ";
	logEntry << tMsg;
	if(tEndLine) logEntry << std::endl;
	gLogStream << logEntry.str();
	gLogStream.flush();
}
void Log::destroyFileLogger() {
	gLogStream.close();
	gInited=false;
	std::fstream src("logs/latest.log", std::ios::in | std::ios::binary);
	if (!src.good()) {
		LOG_ERRR("Couldn't open 'latest.log'");
		return;
	}
	std::string destFileName="logs/" + currentDateTime("%d-%m-%Y %X") + ".log";
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
	HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, tModF+tModB);
	std::cout << tMsg;
	if(tEndLine) std::cout << std::endl;
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
	std::string colorFormat=formatStr("\033[%i;%im",tModF,tModB);
	std::cout << colorFormat << tMsg;
	if(tEndLine) std::cout << std::endl;
#else
	system("ls -l > /dev/null 2>&1");
#endif // !NDEBUG
}
void Log::clear() {
	system("clear");
}
#endif // __linux__

#include "shader.hpp"
Shader Shader::sDefaultShader{};
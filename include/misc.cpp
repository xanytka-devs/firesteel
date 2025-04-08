#include <../include/input/input.hpp>
using namespace Firesteel;

bool Keyboard::mKeys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::mKeysChanged[GLFW_KEY_LAST] = { 0 };

float Mouse::mX = 0;
float Mouse::mY = 0;
float Mouse::m_old_x = 0;
float Mouse::m_old_y = 0;

float Mouse::mDX = 0;
float Mouse::mDY = 0;

float Mouse::mWheelDX = 0;
float Mouse::mWheelDY = 0;

bool Mouse::mFirstMove = true;
bool Mouse::mButtons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::mButtonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };



#include <../include/utils/log.hpp>

#ifdef WIN32
#include <windows.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
bool inited = false;
std::ofstream logStream;

static const std::string currentDateTime(const char* tFormat) {
	struct tm newtime;
	__time64_t long_time;
	char timebuf[26];
	errno_t err;

	// Get time as 64-bit integer.
	_time64(&long_time);
	// Convert to local time.
	err = _localtime64_s(&newtime, &long_time);
	if (err) {
		LOG_WARN("Invalid argument to _localtime64_s.");
		return "invalid";
	}
	strftime(timebuf, sizeof(timebuf), tFormat, &newtime);
	return timebuf;
}

void Log::logToFile(const char* tMsg, const bool tAddTimestamp) {
	if (!inited) {
		if(!std::filesystem::exists("logs/"))
			std::filesystem::create_directory("logs");
		logStream.open("logs/latest.log", std::ofstream::out | std::ofstream::trunc);
		logStream.close();
		logStream.open("logs/latest.log", std::ios::app);
		inited = true;
	}
	std::ostringstream logEntry;
	if(tAddTimestamp) logEntry << currentDateTime("%X") + " ";
	logEntry << tMsg;
	logStream << logEntry.str();
	logStream.flush();
}
void Log::destroyFileLogger() {
	logStream.close();
	inited = false;
	std::fstream src("logs/latest.log", std::ios::in | std::ios::binary);
	if (!src.good()) {
		LOG_ERRR("Couldn't open 'latest.log'");
		return;
	}
	std::string destFileName = "logs/" + currentDateTime("%d-%m-%Y %X") + ".log";
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
void Log::log(const std::string& tMsg, const int tMod, const bool tAddTimestamp) {
	logToFile(tMsg.c_str(), tAddTimestamp); // File logging.
#ifndef NDEBUG
	ShowWindow(GetConsoleWindow(), SW_RESTORE); // Show cmd window.
	SetConsoleTitleA("Firesteel Debug Output"); // Set cmd title.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);// Get cmd handle.
	SetConsoleTextAttribute(hConsole, tMod); // Set cmd text color.
	printf(tMsg.c_str()); // Print msg.
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // !NDEBUG
}
void Log::clear() {
	system("cls");
}
#endif // WIN32
#include <iostream>
#include <windows.h>

#include "firesteel/log.hpp"

namespace firesteel {

#ifndef NDEBUG
	void Log::log(std::string t_msg, const int t_mod) {
		SetConsoleTitleA("firesteel Debug Output"); //Set cmd title.
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//Get cmd handle.
		SetConsoleTextAttribute(hConsole, t_mod); //Set cmd text color.
		printf(t_msg.c_str()); //Print msg.
	}
#endif

}
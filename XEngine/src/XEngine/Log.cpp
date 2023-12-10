#include <iostream>
#include <windows.h>

#include "XEngine/Log.hpp"

namespace XEngine {

	/// <summary>
	/// Logs message to console with specified color and prompt.
	/// </summary>
	/// <param name="t_msg">Text of message.</param>
	/// <param name="t_type">Prompt (INFO, WARN, ERRR, etc.)</param>
	/// <param name="t_mod">Color of text (+ bg, if supported)</param>
	void Log::log(const char* t_msg, const char* t_type, const int t_mod) {
		SetConsoleTitleA("XEngine Debug Output"); //Set cmd title.
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//Get cmd handle.
		SetConsoleTextAttribute(hConsole, t_mod); //Set cmd text color.
		std::cout << "[" << t_type << "] "; //Send prompt.
		//Reset color and print msg.
		SetConsoleTextAttribute(hConsole, 15);
		std::cout << t_msg << std::endl;
	}

}
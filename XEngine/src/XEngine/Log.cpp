#include <iostream>
#include <windows.h>

#include "XEngine/Log.hpp"

namespace XEngine {

#ifdef NDEBUG

#else
	/// <summary>
	/// Logs message to console with specified color and prompt.
	/// </summary>
	/// <param name="t_msg">Text of message.</param>
	/// <param name="t_type">Prompt (INFO, WARN, ERRR, etc.)</param>
	/// <param name="t_mod">Color of text (+ bg, if supported)</param>
	void Log::log(std::string t_msg, std::string t_type, const int t_mod) {
		SetConsoleTitleA("XEngine Debug Output"); //Set cmd title.
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//Get cmd handle.
		SetConsoleTextAttribute(hConsole, t_mod); //Set cmd text color.
		printf(("[" + t_type + "] ").c_str()); //Send prompt.
		//Reset color and print msg.
		SetConsoleTextAttribute(hConsole, 15);
		printf((t_msg + "\n").c_str());
	}
#endif

}
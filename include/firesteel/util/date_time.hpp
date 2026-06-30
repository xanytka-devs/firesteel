#ifndef FS_UTIL_DATE_TIME_H
#define FS_UTIL_DATE_TIME_H

#include <cstdio>
#include <stdexcept>
#include <array>
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif // !WIN32

#include <firesteel/common.hpp>

namespace DateTime {
    static const std::string formatted(const char* tFormat="%d.%m.%Y %X") {      
#ifdef _WIN32
        struct tm newtime;
        __time64_t long_time;
        char timebuf[26];
        errno_t err;
        // Get time as 64-bit integer.
        _time64(&long_time);
        // Convert to local time.
        err=_localtime64_s(&newtime, &long_time);
        if(err) {
            LOG_WARN("DateTime::formatted() was given invalid arguments");
            return "invalid";
        }
        strftime(timebuf, sizeof(timebuf), tFormat, &newtime);
        return timebuf;
#else
        auto t=std::time(nullptr);
        auto tm=*std::localtime(&t);
        return std::put_time(&tm, tFormat)._M_fmt;
#endif // !_WIN32
    }

    static const std::string day() {return formatted("%d");}
    static const std::string dayName() {return formatted("%A");}
    static const std::string month() {return formatted("%m");}
    static const std::string monthName() {return formatted("%B");}
    static const std::string year() {return formatted("%Y");}
    static const std::string time() {return formatted("%X");}
    static const std::string hour() {return formatted("%H");}
    static const std::string minute() {return formatted("%M");}
    static const std::string second() {return formatted("%S");}

    struct Timer {
    	double time=0.0;
    	double limit=60.0;

    	void start() {time=limit;}
    	void reset() {time=0;}
    	void tick() {time-=1;}
    	void tick(double tDT) {time-=tDT;}
        bool isOver() const { return time <= 0; }
    };
}

#endif // !FS_UTIL_DATE_TIME_H
#ifndef FS_UTILS_H
#define FS_UTILS_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#ifdef WIN32
#include <windows.h>
#endif // !WIN32
#ifdef FS_PFD
#include "../external/portable-file-dialogs.hpp"
#endif // FS_PFD

#include "../common.hpp"
#include "UtfConv.hpp"

namespace Color {
    glm::vec3 UIntToRGB(const unsigned int& tR, const unsigned int& tG, const unsigned int& tB) {
        return glm::vec3(tR/255,tG/255,tB/255);
    }
    glm::vec3 HexToRGB(const char* tHex) {
        if(tHex[0] == '#') {
            return glm::vec3(
                std::stoul(std::string("0x") + tHex[1] + tHex[2], nullptr, 4),
                std::stoul(std::string("0x") + tHex[3] + tHex[4], nullptr, 4),
                std::stoul(std::string("0x") + tHex[5] + tHex[6], nullptr, 4)
            );
        } else {
            return glm::vec3(
                std::stoul(std::string("0x") + tHex[0] + tHex[1], nullptr, 4),
                std::stoul(std::string("0x") + tHex[2] + tHex[3], nullptr, 4),
                std::stoul(std::string("0x") + tHex[4] + tHex[5], nullptr, 4)
            );
        }
    }
    glm::vec3 CMYKToRGB(const unsigned int& tC, const unsigned int& tM, const unsigned int& tY, const unsigned int& tK) {
        return glm::vec3(
            (unsigned char)(255 * (1 - tC) * (1 - tK)),
            (unsigned char)(255 * (1 - tM) * (1 - tK)),
            (unsigned char)(255 * (1 - tY) * (1 - tK))
        );
    }
    glm::vec4 UIntToRGBA(const unsigned int tR, const unsigned int& tG, const unsigned int& tB, const unsigned int& tA) {
        return glm::vec4(tR/255,tG/255,tB/255,tA/255);
    }
    glm::vec4 HexToRGBA(const char* tHex) {
        if(tHex[0] == '#') {
            return glm::vec4(
                std::stoul(std::string("0x") + tHex[1] + tHex[2], nullptr, 4),
                std::stoul(std::string("0x") + tHex[3] + tHex[4], nullptr, 4),
                std::stoul(std::string("0x") + tHex[5] + tHex[6], nullptr, 4),
                std::stoul(std::string("0x") + tHex[7] + tHex[8], nullptr, 4)
            );
        } else {
            return glm::vec4(
                std::stoul(std::string("0x") + tHex[0] + tHex[1], nullptr, 4),
                std::stoul(std::string("0x") + tHex[2] + tHex[3], nullptr, 4),
                std::stoul(std::string("0x") + tHex[4] + tHex[5], nullptr, 4),
                std::stoul(std::string("0x") + tHex[6] + tHex[7], nullptr, 4)
            );
        }
    }
}
namespace String {
    std::vector<std::string> split(const std::string& tS, const char& tDelim) {
        std::stringstream ss(tS);
        std::string item;
        std::vector<std::string> out;
        while(std::getline(ss, item, tDelim)) out.push_back(item);
        return out;
    }
    std::string replace(const std::string& tOrig, const char& tChar, const char& tNewChar) {
        std::vector<std::string> parts=split(tOrig, tChar);
        if(parts.size()==1) {
            LOGF_WARN("String::replace(): Couldn't replace '%c' in '%s'\n",tChar,tOrig.c_str());
            return tOrig;
        }
        std::string out=parts[0];
        for(size_t i=1;i<parts.size();i++)
            out+=tNewChar+(i==parts.size())?"":parts[i];
        return out;
    }
    std::string strip(const std::string& tOrig, const char& tChar) {
        return replace(tOrig,tChar,'\0');
    }
    std::string fromFile(const std::string& tPath) {
        if(!std::filesystem::exists(tPath)) {
            LOG_WARN("String::fromFile(): File at \"" + tPath + "\" doesn't exist.");
            return "File at \"" + tPath + "\" doesn't exist.";
        }
        //Open the stream to the file.
        std::ifstream f(tPath, std::ios::in | std::ios::binary);
        const auto sz=std::filesystem::file_size(tPath);
        //Read the whole file into the buffer.
        std::string result(sz, '\0');
        f.read(result.data(), sz);
        return result;
    }
    void toFile(const std::string& tPath, const std::string& tIn) {
        std::ofstream file;
        file.open(tPath);
        file << tIn << std::endl;
        file.close();
    }
    bool endsWith(const char* tStr, const char* tSuffix) {
        if(!tStr||!tSuffix) {
            LOG_WARN("String::endsWith() was given empty arguments");
            return false;
        }
        size_t lenstr=strlen(tStr);
        size_t lensuffix=strlen(tSuffix);
        if(lensuffix>lenstr) {
            LOG_WARN("String::endsWith() was given suffix, bigger than the given string to check");
            return false;
        }
        return strncmp(tStr+lenstr-lensuffix,tSuffix,lensuffix)==0;
    }
    // [!WARNING]
    // Doesn't support UTF8. Use `toLower()` instead.
    std::string toLowerASCII(std::string& tStr) {
    std::transform(tStr.begin(), tStr.end(), tStr.begin(),
        [](unsigned char c) {
            return std::tolower(c);
        });
        return tStr;
    }
    // [!WARNING]
    // Doesn't support UTF8. Use `toUpper()` instead.
    std::string toUpperASCII(std::string& tStr) {
    std::transform(tStr.begin(), tStr.end(), tStr.begin(),
        [](unsigned char c) {
            return std::toupper(c);
        });
        return tStr;
    }
    std::string toLower(const std::string& tStr) {
        return reinterpret_cast<const char*>(Utf8StrMakeLwrUtf8Str(reinterpret_cast<const unsigned char*>(tStr.c_str())));
    }
    std::string toUpper(const std::string& tStr) {
        return reinterpret_cast<const char*>(Utf8StrMakeUprUtf8Str(reinterpret_cast<const unsigned char*>(tStr.c_str())));
    }
}
namespace Math {
#define CASTF(...) static_cast<float>(__VA_ARGS__)
    //Lineary interpolate between `tA` and `tB` by fraction of `tF`.
    float lerp(const float& tA, const float& tB, const float& tF) {
        return tA * (1.f - tF) + (tB * tF);
    }
    glm::vec2 lerp(const glm::vec2& tA, const glm::vec2& tB, const float& tF) {
        return glm::vec2(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF));
    }
    glm::vec3 lerp(const glm::vec3& tA, const glm::vec3& tB, const float& tF) {
        return glm::vec3(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF),lerp(tA.z, tB.z, tF));
    }
    glm::vec4 lerp(const glm::vec4& tA, const glm::vec4& tB, const float& tF) {
        return glm::vec4(lerp(tA.x, tB.x, tF),lerp(tA.y, tB.y, tF),lerp(tA.z, tB.z, tF),lerp(tA.w, tB.w, tF));
    }

    //Cast array to glm::mat4.
    glm::mat4 toMat4(const float** tF) {
        return glm::mat4(
            tF[0][0],tF[0][1],tF[0][2],tF[0][3],
            tF[1][0],tF[1][1],tF[1][2],tF[1][3],
            tF[2][0],tF[2][1],tF[2][2],tF[2][3],
            tF[3][0],tF[3][1],tF[3][2],tF[3][3]
        );
    }
    glm::mat4 toMat4(const double** tF) {
        return glm::mat4(
            CASTF(tF[0][0]),CASTF(tF[0][1]),CASTF(tF[0][2]),CASTF(tF[0][3]),
            CASTF(tF[1][0]),CASTF(tF[1][1]),CASTF(tF[1][2]),CASTF(tF[1][3]),
            CASTF(tF[2][0]),CASTF(tF[2][1]),CASTF(tF[2][2]),CASTF(tF[2][3]),
            CASTF(tF[3][0]),CASTF(tF[3][1]),CASTF(tF[3][2]),CASTF(tF[3][3])
        );
    }
    //Cast std::vector to glm::mat4.
    glm::mat4 toMat4(const std::vector<float>* tF) {
        if(tF->size()<17) return glm::mat4(1);
        glm::mat4 result;
        for(int i=0;i<4;i++)
            result[i*4]=glm::vec4(tF->at(i),tF->at(i+1),tF->at(i+2),tF->at(i+3));
        return result;
    }
    glm::mat4 toMat4(const std::vector<double>* tF) {
        if(tF->size()<17) return glm::mat4(1);
        glm::mat4 result;
        for(int i=0;i<4;i++)
            result[i*4]=glm::vec4(tF->at(i),tF->at(i+1),tF->at(i+2),tF->at(i+3));
        return result;
    }

    //Calculate position on theoretical circle from given radius and angle.
    glm::vec2 posOnCircle(const float& tRad, const float& tAngle) {
        return glm::vec2(tRad*cos(tAngle*(3.14f/180.f)),tRad*sin(tAngle*(3.14f/180.f)));
    }
}
namespace Random {
    void setSeed(const unsigned int& tSeed=0) {
        if(tSeed==0) {
            __time64_t long_time;
	        _time64(&long_time);
	        srand(static_cast<unsigned int>(long_time));
        } else srand(static_cast<unsigned int>(tSeed));
    }
    bool get() {return rand()%1;}
    int get(const int& tMax=INT_MAX) {return rand()%tMax;}
    float get(const float& tMax) {return static_cast<float>(rand())/static_cast<float>(RAND_MAX/tMax);}
    int get(const int& tMin=INT_MIN, const int& tMax=INT_MAX) {return tMin+(rand()%(tMax-tMin));}
    float get(const float& tMin, const float& tMax) {return tMin+static_cast<float>(rand())/static_cast<float>(RAND_MAX/(tMax-tMin));}
}
namespace DateTime {
    static const std::string formatted(const char* tFormat="%d.%m.%Y %X") {
        struct tm newtime;
        __time64_t long_time;
        char timebuf[26];
        errno_t err;
        // Get time as 64-bit integer.
        _time64(&long_time);
        // Convert to local time.
        err=_localtime64_s(&newtime, &long_time);
        if(err) {
            LOG_WARN("formatted() was given invalid arguments");
            return "invalid";
        }
        strftime(timebuf, sizeof(timebuf), tFormat, &newtime);
        return timebuf;
    }

    static const std::string getDay() {return formatted("%d");}
    static const std::string getDayName() {return formatted("%A");}
    static const std::string getMonth() {return formatted("%m");}
    static const std::string getMonthName() {return formatted("%B");}
    static const std::string getYear() {return formatted("%Y");}
    static const std::string getTime() {return formatted("%X");}
    static const std::string getHour() {return formatted("%H");}
    static const std::string getMinute() {return formatted("%M");}
    static const std::string getSecond() {return formatted("%S");}

    struct Timer {
    	double time=0.0;
    	double limit=60.0;

    	void start() {time=limit;}
    	void reset() {time=0;}
    	void tick() {time-=1;}
    	void tick(double tDT) {time-=tDT;}
    	bool isOver() {return time<=0;}
    };
}
namespace OS {
    std::string executeInCmd(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(
#ifdef WIN32
            &_pclose
#endif // !WIN32
#ifdef __linux__
            &pclose
#endif // !__linux__
            )> pipe(
#ifdef WIN32
                _popen(cmd, "r"),
#endif // !WIN32
#ifdef __linux__
                popen(cmd, "r"),
#endif // !__linux__
#ifdef WIN32
                &_pclose
#endif // !WIN32
#ifdef __linux__
                &pclose
#endif // !__linux__
            );
        if(!pipe) {
            LOG_ERRR("Couldn't execute in cmd because popen() failed");
            return "failed";
        }
        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
    static void openURL(const char* tUrl) {
#ifdef WIN32
        ShellExecuteA(NULL, "open", tUrl, NULL, NULL, SW_SHOWNORMAL);
#endif // !WIN32
#ifdef __linux__
        system(tUrl);
#endif // !__linux__
    }
#ifdef FS_PFD
    /// Calls system dialog to save or open files.
    /// @param `tSave` determines if dialog will require existing files to open or files to save to.
    /// @param `tAllowForceful` determines if OS allows user to multiselect files in open file dialog or if it forces OS to overwrite files in save file dialog.
    /// @param `tDefPath` determines dialog starting location.
    /// @param `tFilters` determines what files will be shown by files.
    /// @param `tTitle` determines title of file dialog.
    std::vector<std::string> fileDialog(const bool& tSave, const bool& tAllowForceful,
        std::string tDefPath="", std::vector<std::string>* tFilters=nullptr, const char* tTitle=nullptr) {
        //Get path.
        if(tDefPath=="") tDefPath=pfd::path::home().c_str();
        //Get filters.
        std::vector<std::string> filters{"All Files","*"};
        if(tFilters) filters=*tFilters;
        //Get title.
        if(!tTitle) {
            if(tSave) tTitle="Save file to...";
            else tTitle="Open file";
        }
        //Do the operation.
        if(tSave) {
            auto f = pfd::save_file(tTitle, tDefPath, filters, tAllowForceful?pfd::opt::force_overwrite:pfd::opt::none);
            return {f.result()};
        }
        auto f = pfd::open_file(tTitle, tDefPath, filters, tAllowForceful?pfd::opt::multiselect:pfd::opt::none);
        return f.result();
    }
    /// Calls system dialog to save or open folder.
    /// @param `tDefPath` determines dialog starting location.
    /// @param `tTitle` determines title of folder dialog.
    std::string folderDialog(std::string tDefPath="", const char* tTitle=nullptr) {
        //Get path.
        if(tDefPath=="") tDefPath=pfd::path::home().c_str();
        //Get title.
        if(!tTitle) tTitle="Open folder";
        //Do the operation.
        auto f = pfd::select_folder(tTitle, tDefPath);
        return f.result();
    }
    enum MessageBoxChoice {
        MBC_OK = 0,
        MBC_OK_CANCEL,
        MBC_YES_NO,
        MBC_YES_NO_CANCEL,
        MBC_RETRY_CANCEL,
        MBC_ABORT_RETRY_IGNORE
    };
    struct MessageBoxMethods {
        void(*clickedOk)()=nullptr;
        void(*clickedCancel)()=nullptr;
        void(*clickedYes)()=nullptr;
        void(*clickedNo)()=nullptr;
        void(*clickedRetry)()=nullptr;
        void(*clickedAbort)()=nullptr;
        void(*clickedIgnore)()=nullptr;
    };
    enum Icon {
        ICON_INFO = 0,
        ICON_WARNING,
        ICON_ERROR,
        ICON_QUESTION
    };
    /// Calls system message box.
    /// @param `tContent` determines the message.
    /// @param `tChoice` determines choices that are given to user.
    /// @param `tMethods` determines callbacks that will be run depending on user input.
    /// @param `tIcon` determines icon in the message box.
    /// @param `tTitle` determines title of message box.
    void messageBox(const char* tContent,
        const MessageBoxChoice& tChoice=MBC_OK, const MessageBoxMethods& tMethods={}, const Icon& tIcon=ICON_INFO, const char* tTitle=nullptr) {
        //Do the operation.
        auto m=pfd::message(tTitle, tContent, (pfd::choice)tChoice, (pfd::icon)tIcon);
        //Perfome actions depending on result.
        switch(m.result()) {
        case pfd::button::ok:
            if(tMethods.clickedOk) tMethods.clickedOk();
            break;
        case pfd::button::cancel:
            if(tMethods.clickedCancel) tMethods.clickedCancel();
            break;
        case pfd::button::yes:
            if(tMethods.clickedYes) tMethods.clickedYes();
            break;
        case pfd::button::no:
            if(tMethods.clickedNo) tMethods.clickedNo();
            break;
        case pfd::button::retry:
            if(tMethods.clickedRetry) tMethods.clickedRetry();
            break;
        case pfd::button::abort:
            if(tMethods.clickedOk) tMethods.clickedAbort();
            break;
        case pfd::button::ignore:
            if(tMethods.clickedIgnore) tMethods.clickedIgnore();
            break;
        }
    }
    /// Calls system notification box.
    /// @param `tTitle` determines title of notification.
    /// @param `tContent` determines the message.
    /// @param `tIcon` determines icon in the notification.
    void notification(const char* tTitle, const char* tContent, const Icon& tIcon=ICON_INFO) {
        pfd::notify(tTitle, tContent, (pfd::icon)tIcon);
    }
#endif // FS_PFD
}

/// @deprecated Use OS\::fileDialog() instead.
struct FileDialog {
    // Deafult return value (if file isn't gotten).
    std::string default_file="";
    // Path to start in.
    const char* path=NULL;
    // Extension filter.
    const char* filter="All\0*.*\0";
    // Index of filter's item.
    int filter_id=0;
    // Opens file dialog and gets file name. Otherwise returns set default.
    std::string open() const;
    // Opens file dialog and gets file name. Otherwise returns set default.
    std::string save() const;
};
std::string FileDialog::open() const {
#ifdef WIN32
    OPENFILENAME ofn;           //Common dialog box structure.
    char szFile[MAX_PATH]{};    //Buffer for file name.
    HWND hwnd=nullptr;          //Owner window.
    HANDLE hf=nullptr;          //File handle.
    //Initialize OPENFILENAME.
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize=sizeof(ofn);
    ofn.hwndOwner=hwnd;
    ofn.lpstrFile=szFile;
    //Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0]='\0';
    ofn.nMaxFile=sizeof(szFile);
    ofn.lpstrDefExt="rle";
    ofn.lpstrFilter=filter;
    ofn.nFilterIndex=filter_id;
    ofn.lpstrFileTitle=NULL;
    ofn.nMaxFileTitle=0;
    ofn.lpstrInitialDir=path;
    ofn.Flags=OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    //Display the Open dialog box. 
    if (GetOpenFileName(&ofn) == TRUE)
        return szFile;
    return default_file;
#endif // !WIN32
#ifdef __linux__
    //Currently isn't implemented.
    return "";
#endif // !__linux__
}
std::string FileDialog::save() const {
#ifdef WIN32
    OPENFILENAME ofn;           //Common dialog box structure.
    char szFile[MAX_PATH]{};    //Buffer for file name.
    HWND hwnd=nullptr;          //Owner window.
    HANDLE hf=nullptr;          //File handle.
    //Initialize OPENFILENAME.
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize=sizeof(ofn);
    ofn.hwndOwner=hwnd;
    ofn.lpstrFile=szFile;
    //Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0]='\0';
    ofn.nMaxFile=sizeof(szFile);
    ofn.lpstrDefExt="rle";
    ofn.lpstrFilter=filter;
    ofn.nFilterIndex=filter_id;
    ofn.lpstrFileTitle=NULL;
    ofn.nMaxFileTitle=0;
    ofn.lpstrInitialDir=path;
    ofn.Flags=OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    //Display the Open dialog box. 
    if (GetSaveFileName(&ofn) == TRUE) {
        return szFile;
    }
    return default_file;
#endif // !WIN32
#ifdef __linux__
    return "";
#endif // !__linux__
}

/// @deprecated Use OS\::messageBox() instead.
static void showMessageBox(GLFWwindow* tWin, std::string tMsg, std::string tTitle="Firesteel Message Box") {
#ifdef WIN32
    MessageBox(glfwGetWin32Window(tWin), tMsg.c_str(), tTitle.c_str(), MB_OK | MB_ICONQUESTION);
#endif // !WIN32
#ifdef __linux__
    // Of course there's SDL2s message box API, but it needs SDL2 so I won't implement it for now.
    LOG_INFO("Currently Message Boxes are not implemented for the Linux OS.");
#endif // !__linux__
#ifdef __APPLE__
    // Of course there's SDL2s message box API, but it needs SDL2 so I won't implement it for now.
    LOG_INFO("Currently Message Boxes are not implemented for the MacOS.");
#endif // !__APPLE__
}

#endif // !FS_UTILS_H
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
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "../common.hpp"
#include "UtfConv.hpp"

void genRandom() {
    __time64_t long_time;
	_time64(&long_time);
	srand(static_cast<unsigned int>(long_time));
}
bool getRandom() {
    return rand()%1;
}
int getRandom(int tMax) {
    return rand()%tMax;
}
float getRandom(float tMax) {
    return static_cast<float>(rand()) / (static_cast <float> (RAND_MAX/tMax));
}
int getRandom(int tMin, int tMax) {
    return (rand()%tMax)+tMin;
}
float getRandom(float tMin, float tMax) {
    return tMin+static_cast<float>(rand()) / (static_cast <float> (RAND_MAX/(tMax-tMin)));
}

float lerp(float a, float b, float f) {
    return a * (1.f - f) + (b * f);
}
glm::vec3 float3ToVec3(float* tF) {
    return glm::vec3(tF[0], tF[1], tF[2]);
}

glm::vec3 UIntToRGB(unsigned int tR, unsigned int tG, unsigned int tB) {
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
glm::vec3 CMYKToRGB(unsigned int tC, unsigned int tM, unsigned int tY, unsigned int tK) {
    return glm::vec3(
        (unsigned char)(255 * (1 - tC) * (1 - tK)),
        (unsigned char)(255 * (1 - tM) * (1 - tK)),
        (unsigned char)(255 * (1 - tY) * (1 - tK))
    );
}

glm::vec4 UIntToRGBA(unsigned int tR, unsigned int tG, unsigned int tB, unsigned int tA) {
    return glm::vec4(tR / 255, tG / 255, tB / 255, tA / 255);
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

std::vector<std::string> StrSplit(const std::string& tS, char tDelim) {
    std::stringstream ss(tS);
    std::string item;
    std::vector<std::string> out;
    while (std::getline(ss, item, tDelim)) {
        out.push_back(item);
    }
    return out;
}

std::string StrReplace(std::string tOrig, char tChar, char tNewChar) {
    std::vector<std::string> parts = StrSplit(tOrig, tChar);
    std::string out = parts[0];
    for (size_t i = 1; i < parts.size(); i++)
        out += tNewChar + parts[i];
    return out;
}

std::string StrFromFile(std::string tPath) {
    std::filesystem::path path(tPath);
    if(!std::filesystem::exists(path)) {
        LOG_WARN("File at \"" + tPath + "\" doesn't seem to exist.");
        return "File at \"" + tPath + "\" doesn't seem to exist.";
    }
    // Open the stream to 'lock' the file.
    std::ifstream f(path, std::ios::in | std::ios::binary);
    // Obtain the size of the file.
    const auto sz = std::filesystem::file_size(path);
    // Create a buffer.
    std::string result(sz, '\0');
    // Read the whole file into the buffer.
    f.read(result.data(), sz);
    return result;
}

void StrToFile(std::string tPath, std::string tIn) {
    std::ofstream file;
    file.open(tPath);
    file << tIn << std::endl;
    file.close();
}

int StrEndsWith(const char* tStr, const char* tSuffix) {
    if (!tStr || !tSuffix)
        return 0;
    size_t lenstr = strlen(tStr);
    size_t lensuffix = strlen(tSuffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(tStr + lenstr - lensuffix, tSuffix, lensuffix) == 0;
}

// !!! NOT UTF8 !!!
// Use StrToLower instead.
std::string StrASCIIToLower(std::string tStr) {
    std::transform(tStr.begin(), tStr.end(), tStr.begin(),
        [](unsigned char c) {
            return std::tolower(c);
        });
    return tStr;
}
// !!! NOT UTF8 !!!
// Use StrToUpper instead.
std::string StrASCIIToUpper(std::string tStr) {
    std::transform(tStr.begin(), tStr.end(), tStr.begin(),
        [](unsigned char c) {
            return std::toupper(c);
        });
    return tStr;
}

static const std::string currentDateTime(const char* tFormat = "%d.%m.%Y %X") {
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

std::string StrToLower(std::string tStr) {
    return reinterpret_cast<const char*>(Utf8StrMakeLwrUtf8Str(reinterpret_cast<const unsigned char*>(tStr.c_str())));
}
std::string StrToUpper(std::string tStr) {
    return reinterpret_cast<const char*>(Utf8StrMakeUprUtf8Str(reinterpret_cast<const unsigned char*>(tStr.c_str())));
}

/// Dialog for file (open/save).
struct FileDialog {
    // Deafult return value (if file isn't gotten).
    std::string default_file = "";
    // Path to start in.
    const char* path = NULL;
    // Extension filter.
    const char* filter = "All\0*.*\0";
    // Index of filter's item.
    int filter_id = 0;
    // Opens file dialog and gets file name. Otherwise returns set default.
    std::string open() const;
    // Opens file dialog and gets file name. Otherwise returns set default.
    std::string save() const;
};

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
    /// @param `tAllowMultiselect` determines if OS allows user to multiselect files in file dialog (ONLY WORKS FOR NON-SAVE DIALOG).
    /// @param `tDefPath` determines dialog starting location.
    /// @param `tFilters` determines what files will be shown by files.
    /// @param `tTitle` determines title of file dialog.
    std::vector<std::string> fileDialog(const bool& tSave, const bool& tAllowMultiselect,
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
            auto f = pfd::save_file(tTitle, tDefPath, filters, tAllowMultiselect);
            return {f.result()};
        }
        auto f = pfd::open_file(tTitle, tDefPath, filters, tAllowMultiselect);
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
#ifndef FS_UTILS_H
#define FS_UTILS_H

#include <cstdlib>

// Randomizes current rand() seed.
void genRandom() {
    __time64_t long_time;
	_time64(&long_time);
	srand(static_cast<unsigned int>(long_time));
}

// Returns random number.
bool getRandom() {
    genRandom();
    return rand()%1;
}
// Returns random number.
int getRandom(int tMax) {
    genRandom();
    return rand()%tMax;
}
// Returns random number.
float getRandom(float tMax) {
    genRandom();
    return static_cast<float>(rand()) / (static_cast <float> (RAND_MAX/tMax));
}
// Returns random number.
int getRandom(int tMin, int tMax) {
    genRandom();
    return (rand()%tMax)+tMin;
}
// Returns random number.
float getRandom(float tMin, float tMax) {
    genRandom();
    return tMin+static_cast<float>(rand()) / (static_cast <float> (RAND_MAX/(tMax-tMin)));
}

float lerp(float a, float b, float f) {
    return a * (1.f - f) + (b * f);
}

#include <glm/vec3.hpp>
#include <string>

glm::vec3 UIntToRGB(unsigned int tR, unsigned int tG, unsigned int tB) {
    return glm::vec3(tR, tG, tB) / 255.f;
}

glm::vec3 HexToRGB(std::string tHex) {
    if(tHex[0] == '#') tHex.erase(0, 1);
    int r, g, b;
    sscanf(tHex.c_str(), "%02x%02x%02x", &r, &g, &b);
    return glm::vec3(r, g, b)/ 255.f;
}

glm::vec3 CMYKToRGB(unsigned int tC, unsigned int tM, unsigned int tY, unsigned int tK) {
    return glm::vec3(
        (unsigned char)(255 * (1 - tC) * (1 - tK)),
        (unsigned char)(255 * (1 - tM) * (1 - tK)),
        (unsigned char)(255 * (1 - tY) * (1 - tK))
    );
}

#include <glm/vec4.hpp>

glm::vec4 UIntToRGBA(unsigned int tR, unsigned int tG, unsigned int tB, unsigned int tA) {
    return glm::vec4(tR / 255.f, tG / 255.f, tB / 255.f, tA / 255.f);
}

glm::vec4 HexToRGBA(std::string tHex) {
    if(tHex[0] == '#') tHex.erase(0, 1);
    int r, g, b, a;
    sscanf(tHex.c_str(), "%02x%02x%02x%02x", &r, &g, &b, &a);
    return glm::vec4(r, g, b, a)/ 255.f;
}

#include <iostream>
#include <fstream>
#include <filesystem>
#include "../common.hpp"

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

glm::vec3 float3ToVec3(float* tF) {
    return glm::vec3(tF[0], tF[1], tF[2]);
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

#include "UtfConv.hpp"
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

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#ifdef WIN32
#include <windows.h>
#endif // !WIN32
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
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
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
std::string FileDialog::open() const {
#ifdef WIN32
    OPENFILENAME ofn;       //Common dialog box structure.
    char szFile[MAX_PATH]{};     //Buffer for file name.
    HWND hwnd = nullptr;    //Owner window.
    HANDLE hf = nullptr;    //File handle.
    //Initialize OPENFILENAME.
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    //Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrDefExt = "rle";
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = filter_id;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = path;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    //Display the Open dialog box. 
    if (GetOpenFileName(&ofn) == TRUE)
        return szFile;
    return default_file;
#endif // !WIN32
#ifdef __linux__
    return "";
#endif // !__linux__
}
std::string FileDialog::save() const {
#ifdef WIN32
    OPENFILENAME ofn;       //Common dialog box structure.
    char szFile[MAX_PATH]{};     //Buffer for file name.
    HWND hwnd = nullptr;    //Owner window.
    HANDLE hf = nullptr;    //File handle.
    //Initialize OPENFILENAME.
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    //Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
    // use the contents of szFile to initialize itself.
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrDefExt = "rle";
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = filter_id;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = path;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
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
static void showMessageBox(GLFWwindow* tWin, std::string tMsg, std::string tTitle = "Firesteel Message Box") {
#ifdef WIN32
    MessageBox(glfwGetWin32Window(tWin), tMsg.c_str(), tTitle.c_str(), MB_OK | MB_ICONQUESTION);
#endif // !WIN32
#ifdef __linux__
    // Of course there's SDL2s message box API, but it needs SDL2 so I won't implement it for now.
    LOG_INFO("Currently Message Boxes are not implemented for the Linux OS.");
#endif // !__linux__
}

#endif // !FS_UTILS_H
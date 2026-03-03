#ifndef FS_UTIL_STRING_H
#define FS_UTIL_STRING_H

#include <iostream>
#include <fstream>
#include <filesystem>

#include <firesteel/common.hpp>
#include <firesteel/utils/utf_conv.hpp>

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
            LOGF_WARN("String::replace(): Couldn't replace '%c' in '%s'",tChar,tOrig.c_str());
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
    bool endsWith(const std::string& tStr, const char* tSuffix) {
        return endsWith(tStr.c_str(), tSuffix);
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

#endif // !FS_UTIL_STRING_H
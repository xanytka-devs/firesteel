#ifndef FS_INTERNAL_SYSTEMSPECS_H
#define FS_INTERNAL_SYSTEMSPECS_H

#include <string>

struct CPUInfo {
    const char* output="";
    std::string model="";
    std::string vendor="";
    unsigned int cores=0;
    unsigned int frequency=0;
};
CPUInfo getCPUInfo();

struct GPUInfo {
    const char* output="";
    std::string model="";
    double memoryGB=0;
};
GPUInfo getGPUInfo();

struct RAMInfo {
    const char* output="";
    double memoryGB=0;
};
RAMInfo getRAMInfo();

struct MotherboardInfo {
    const char* output="";
    std::string model="";
    std::string vendor="";
};
MotherboardInfo getMotherboardInfo();

struct OSInfo {
    std::string name="";
    std::string version="";
    std::string architecture="";
    std::string distroBuild="";
};
OSInfo getOSInfo();

#endif // !FS_INTERNAL_SYSTEMSPECS_H
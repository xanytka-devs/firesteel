#ifndef FS_INTERNAL_SYSTEMSPECS
#define FS_INTERNAL_SYSTEMSPECS

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")
#pragma warning(disable : 4996)
#else
#include <unistd.h>
#include <sys/utsname.h>
#endif

namespace INTERNAL {
    std::string exec_command(const std::string& cmd) {
        char buffer[128];
        std::string result = "";
#ifdef _WIN32
        FILE* pipe = _popen(cmd.c_str(), "r");
#else
        FILE* pipe = popen(cmd.c_str(), "r");
#endif
        if (!pipe) return "ERROR";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
#ifdef _WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
        return result;
    }
}

struct CPUInfo {
    const char* output="";
    std::string model;
    std::string vendor;
    unsigned int cores;
    unsigned int frequency;
};
CPUInfo getCPUInfo() {
    CPUInfo out;
#ifdef _WIN32
    //For Windows using WMI.
    HRESULT hres;
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        out.output="WMI initialization failed";
        return out;
    }

    hres = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE, NULL
    );
    if (FAILED(hres)) {
        CoUninitialize();
        out.output = "Security initialization failed";
        return out;
    }

    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc
    );
    if (FAILED(hres)) {
        CoUninitialize();
        out.output = "Failed to create IWbemLocator";
        return out;
    }

    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), NULL, NULL,
        0, NULL, 0, 0, &pSvc
    );
    if (FAILED(hres)) {
        pLoc->Release();
        CoUninitialize();
        out.output = "Failed to connect to WMI";
        return out;
    }

    hres = CoSetProxyBlanket(
        pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
        NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE
    );
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        out.output = "Failed to set proxy blanket";
        return out;
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"), bstr_t("SELECT * FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnumerator
    );
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        out.output = "Query failed";
        return out;
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            out.model = std::string(_bstr_t(vtProp.bstrVal));
            VariantClear(&vtProp);
        }

        hr = pclsObj->Get(L"NumberOfCores", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            out.cores = vtProp.uintVal;
            VariantClear(&vtProp);
        }

        hr = pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            out.frequency = vtProp.uintVal;
            VariantClear(&vtProp);
        }

        hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            out.vendor = std::string(_bstr_t(vtProp.bstrVal));
            VariantClear(&vtProp);
        }

        pclsObj->Release();
    }
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
#else
    //For Linux using '/proc/cpuinfo'.
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (cpuinfo.is_open()) {
        std::string line;
        while (getline(cpuinfo, line)) {
            if (line.find("model name") != std::string::npos)
                out.model = line.substr(line.find(":") + 2);
            else if (line.find("cpu cores") != std::string::npos)
                out.cores = std::stoul(line.substr(line.find(":") + 2));
            else if (line.find("cpu MHz") != std::string::npos)
                out.frequency = std::stoul(line.substr(line.find(":") + 2));
            else if (line.find("vendor_id") != std::string::npos)
                out.vendor = line.substr(line.find(":") + 2);
        }
        cpuinfo.close();
    } else 
        out.output = "Failed to open /proc/cpuinfo";
#endif
    return out;
}

struct GPUInfo {
    const char* output="";
    std::string model;
    double memoryGB;
};
GPUInfo getGPUInfo() {
    GPUInfo out;
#ifdef _WIN32
    //For Windows using WMI.
    HRESULT hres;
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        out.output = "WMI initialization failed";
        return out;
    }

    hres = CoInitializeSecurity(
        NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL
    );

    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc
    );

    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0,
        NULL, 0, 0, &pSvc
    );

    hres = CoSetProxyBlanket(
        pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
        NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE
    );

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"), bstr_t("SELECT * FROM Win32_VideoController"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnumerator
    );

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            out.model = std::string(_bstr_t(vtProp.bstrVal));
            VariantClear(&vtProp);
        }

        hr = pclsObj->Get(L"AdapterRAM", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            double ramGB = vtProp.uintVal / (1024.0 * 1024.0 * 1024.0);
            out.memoryGB = ramGB;
            VariantClear(&vtProp);
        }

        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
#else
    //For Linux using lspci.
    std::string lspci = INTERNAL::exec_command("lspci | grep -i vga");
    if (!lspci.empty()) {
        size_t pos = lspci.find(": ");
        if (pos != std::string::npos) {
            out.model = lspci.substr(pos + 2);
        }
    }

    //Try to get memory by glxinfo.
    std::string glxinfo = INTERNAL::exec_command("glxinfo -B | grep -i 'Video memory'");
    if (!glxinfo.empty()) {
        size_t pos = glxinfo.find(": ");
        if (pos != std::string::npos) {
            out.memoryGB = std::stoul(glxinfo.substr(pos + 2));
        }
    }
#endif
    return out;
}

struct RAMInfo {
    const char* output="";
    double memoryGB;
};
RAMInfo getRAMInfo() {
    RAMInfo out;
#ifdef _WIN32
    //For Windows using GlobalMemoryStatusEx.
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    if (GlobalMemoryStatusEx(&memInfo)) {
        double totalGB = memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
        out.memoryGB = totalGB;
    } else
        out.output = "Failed to get memory info";
#else
    //For Linux using '/proc/meminfo'.
    std::ifstream meminfo("/proc/meminfo");
    if (meminfo.is_open()) {
        std::string line;
        while (getline(meminfo, line)) {
            if (line.find("MemTotal") != std::string::npos) {
                size_t pos = line.find(":");
                std::string value = line.substr(pos + 1);
                value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
                double totalGB = stod(value) / 1024.0 / 1024.0;
                out.memoryGB = totalGB;
                break;
            }
        }
        meminfo.close();
    } else
        out.output = "Failed to open /proc/meminfo";
#endif
    return out;
}

struct MotherboardInfo {
    const char* output="";
    std::string model;
    std::string vendor;
};
MotherboardInfo getMotherboardInfo() {
    MotherboardInfo out;
#ifdef _WIN32
    //For Windows using WMI.
    HRESULT hres;
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        out.output = "WMI initialization failed";
        return out;
    }

    hres = CoInitializeSecurity(
        NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL
    );

    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc
    );

    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0,
        NULL, 0, 0, &pSvc
    );

    hres = CoSetProxyBlanket(
        pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE
    );

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"), bstr_t("SELECT * FROM Win32_BaseBoard"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnumerator
    );

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;

    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            out.vendor = std::string(_bstr_t(vtProp.bstrVal));
            VariantClear(&vtProp);
        }

        hr = pclsObj->Get(L"Product", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            out.model = std::string(_bstr_t(vtProp.bstrVal));
            VariantClear(&vtProp);
        }

        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();
#else
    //For Linux using dmidecode (with sudo).
    std::string dmidecode = INTERNAL::exec_command("sudo dmidecode -t baseboard");
    if (!dmidecode.empty()) {
        size_t man_pos = dmidecode.find("Manufacturer:");
        if (man_pos != std::string::npos) {
            size_t end_line = dmidecode.find("\n", man_pos);
            out.vendor = dmidecode.substr(man_pos, end_line - man_pos);
        }

        size_t prod_pos = dmidecode.find("Product Name:");
        if (prod_pos != std::string::npos) {
            size_t end_line = dmidecode.find("\n", prod_pos);
            out.model = dmidecode.substr(prod_pos, end_line - prod_pos);
        }
    }
    else
        out.output = "Failed to get motherboard info (try with sudo)";
#endif
    return out;
}

struct OSInfo {
    std::string name;
    std::string version;
    std::string architecture;
    std::string distroBuild;
};
OSInfo getOSInfo() {
    OSInfo out;
#ifdef _WIN32
    //For Windows.
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
        out.name = "Windows";
        out.distroBuild = std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion);
        out.version = std::to_string(osvi.dwBuildNumber);
    }

    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    switch (si.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64: out.architecture = "x64"; break;
    case PROCESSOR_ARCHITECTURE_ARM: out.architecture = "ARM"; break;
    case PROCESSOR_ARCHITECTURE_IA64: out.architecture = "IA-64"; break;
    case PROCESSOR_ARCHITECTURE_INTEL: out.architecture = "x86"; break;
    default: out.architecture = "Unknown";
    }
#else
    //For Linux.
    struct utsname unameData;
    if (uname(&unameData) == 0) {
        out.os = std::string(unameData.sysname);
        out.version = std::string(unameData.release);
        out.architecture = std::string(unameData.machine);
    }

    //Get version for Linux.
    std::ifstream os_release("/etc/os-release");
    if (os_release.is_open()) {
        std::string line;
        while (getline(os_release, line)) {
            if (line.find("PRETTY_NAME") != std::string::npos) {
                size_t start = line.find("\"") + 1;
                size_t end = line.rfind("\"");
                if (start != std::string::npos && end != std::string::npos)
                    out.distroBuild = line.substr(start, end - start);
                break;
            }
        }
        os_release.close();
    }
#endif
    return out;
}

#endif // !FS_INTERNAL_SYSTEMSPECS
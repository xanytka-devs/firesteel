#ifndef FS_INTERNAL_CONFIG_H
#define FS_INTERNAL_CONFIG_H

#include "systemspecs.hpp"
#include "../utils/log.hpp"
#ifndef FS_NO_JSON
#include "../utils/json.hpp"
#endif // !FS_NO_JSON

namespace Firesteel {
    namespace CONFIG {
        static bool sAllowDevView;
	    void checkGlobalFile() {
            // Generic data retrieval (for better understanding of logs, etc.).
            std::string firesteelConfigFile="";
            bool canGetSystemInfo=true;
#if defined(_WIN32) && !defined(FS_NO_JSON)
            char* buf=nullptr;
            size_t sz=0;
            if(_dupenv_s(&buf, &sz, "APPDATA") == 0 && buf != nullptr) {
                firesteelConfigFile=std::string(buf) + "\\firesteel\\global.cfg.json";
                free(buf);
            }
            if(std::filesystem::exists(firesteelConfigFile)) {
                LOG_INFO("Found global Firesteel config. Retrieving...");
                std::ifstream ifs(firesteelConfigFile);
                nlohmann::json cfg=nlohmann::json::parse(ifs);
                ifs.close();
                if(!cfg["AllowDevView"].is_null()) sAllowDevView=cfg["AllowDevView"];
                if(!cfg["AllowHardwareEnumeration"].is_null()) canGetSystemInfo=cfg["AllowHardwareEnumeration"];
                if(!cfg["SaveLogs"].is_null()) Log::sSaveLogs=cfg["SaveLogs"];
            }
#endif
            if(canGetSystemInfo) {
                LOG_INFO("Hardware Information:");

                CPUInfo cpu=getCPUInfo();
                LOG_INFO(" - CPU");
                if(std::string(cpu.output).size() <= 1) {
                    LOG_INFO("   Vendor: " + cpu.vendor);
                    LOG_INFO("   Model: " + cpu.model);
                    LOGF_INFO("   Cores: %d", cpu.cores);
                    LOGF_INFO("   Frequency: %d", cpu.frequency);
                } else LOG_WARN(cpu.output);

                GPUInfo gpu=getGPUInfo();
                LOG_INFO(" - GPU");
                if(std::string(gpu.output).size() <= 1) {
                    LOG_INFO("   Model: " + gpu.model);
                    LOGF_INFO("   Memory: %.2f GB", gpu.memoryGB);
                } else LOG_WARN(gpu.output);

                RAMInfo ram=getRAMInfo();
                LOG_INFO(" - RAM");
                if(std::string(ram.output).size() <= 1) {
                    LOGF_INFO("   Memory: %.2f GB", ram.memoryGB);
                } else LOG_WARN(ram.output);

                MotherboardInfo motherboard=getMotherboardInfo();
                LOG_INFO(" - Motherboard");
                if(std::string(motherboard.output).size() <= 1) {
                    LOG_INFO("   Model: " + motherboard.model);
                    LOG_INFO("   Vendor: " + motherboard.vendor);
                } else LOG_WARN(motherboard.output);

                OSInfo os=getOSInfo();
                LOG_INFO(" - OS");
                LOG_INFO("   Name: " + os.name);
                LOG_INFO("   Build/distro: " + os.distroBuild);
                LOG_INFO("   Version: " + os.version);
                LOG_INFO("   Architecture: " + os.architecture);
            }
	    }
    }
}

#endif // !FS_INTERNAL_CONFIG_H
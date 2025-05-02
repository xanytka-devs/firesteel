#ifndef FS_INTERNAL_CONFIG
#define FS_INTERNAL_CONFIG

#include "systemspecs.hpp"
#include "../utils/log.hpp"
#include "../utils/json.hpp"

namespace Firesteel {
    namespace CONFIG {
	    void checkGlobalFile() {
            // Generic data retrieval (for better understanding of logs, etc.).
            std::string firesteelConfigFile = std::string(getenv("APPDATA")) + "\\firesteel\\global.cfg.json";
            bool canGetSystemInfo = true;
            if (std::filesystem::exists(firesteelConfigFile)) {
                LOG_INFO("Found global Firesteel config. Retrieving...");
                std::ifstream ifs(firesteelConfigFile);
                nlohmann::json cfg = nlohmann::json::parse(ifs);
                if (!cfg["AllowHardwareEnumeration"].is_null()) canGetSystemInfo = cfg["AllowHardwareEnumeration"];
            }
            if(canGetSystemInfo) {
                LOG_INFO("Hardware Information:");

                CPUInfo cpu = getCPUInfo();
                LOG_INFO(" - CPU");
                if(cpu.output == "") {
                    LOG_INFO("   Vendor: " + cpu.vendor);
                    LOG_INFO("   Model: " + cpu.model);
                    LOG_INFO("   Cores: " + std::to_string(cpu.cores));
                    LOG_INFO("   Frequency: " + std::to_string(cpu.frequency));
                } else LOG_WARN(cpu.output);

                GPUInfo gpu = getGPUInfo();
                LOG_INFO(" - GPU");
                if(gpu.output == "") {
                    LOG_INFO("   Model: " + gpu.model);
                    LOG_INFO("   Memory: " + std::to_string(gpu.memoryGB) + " GB");
                } else LOG_WARN(gpu.output);

                RAMInfo ram = getRAMInfo();
                LOG_INFO(" - RAM");
                if(ram.output == "") {
                    LOG_INFO("   Memory: " + std::to_string(ram.memoryGB) + " GB");
                } else LOG_WARN(ram.output);

                MotherboardInfo motherboard = getMotherboardInfo();
                LOG_INFO(" - Motherboard");
                if(motherboard.output == "") {
                    LOG_INFO("   Model: " + motherboard.model);
                    LOG_INFO("   Vendor: " + motherboard.vendor);
                } else LOG_WARN(motherboard.output);

                OSInfo os = getOSInfo();
                LOG_INFO(" - OS");
                LOG_INFO("   Name: " + os.name);
                LOG_INFO("   Build/distro: " + os.distroBuild);
                LOG_INFO("   Version: " + os.version);
                LOG_INFO("   Architecture: " + os.architecture);
            }
	    }
    }
}

#endif // !FS_INTERNAL_CONFIG
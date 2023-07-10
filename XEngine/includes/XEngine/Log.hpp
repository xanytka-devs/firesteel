#pragma once

#include <spdlog/spdlog.h>

namespace XEngine {

#ifdef NDEBUG
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERRR(...)
#define LOG_CRIT(...)
#else
#define LOG_INFO(...)	spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)	spdlog::warn(__VA_ARGS__)
#define LOG_ERRR(...)	spdlog::error(__VA_ARGS__)
#define LOG_CRIT(...)	spdlog::critical(__VA_ARGS__)
#endif

}
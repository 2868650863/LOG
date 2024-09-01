#pragma once

#include "logger.hpp"


namespace logsys
{
    Logger::ptr_logger getLogger(const string &name)
    {
        return LoggerManager::getInstance().getLogger(name);
    }
    Logger::ptr_logger rootLogger()
    {
        return LoggerManager::getInstance().rootLogger();
    }
    // 代理模式
    // VA_ARGS为空时  ## 会自动移除前面的逗号，避免编译错误
#define debug(fmt, ...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define info(fmt, ...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define warn(fmt, ...) warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define error(fmt, ...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define fatal(fmt, ...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

//#define DEBUG(logger, fmt, ...) logger->debug(fmt, ##__VA_ARGS__)
#define DEBUG(fmt, ...) logsys::rootLogger()->debug(fmt, ##__VA_ARGS__)
#define INFO(fmt, ...) logsys::rootLogger()->info(fmt, ##__VA_ARGS__)
#define WARN(fmt, ...) logsys::rootLogger()->warn(fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) logsys::rootLogger()->error(fmt, ##__VA_ARGS__)
#define FATAL(fmt, ...) logsys::rootLogger()->fatal(fmt, ##__VA_ARGS__)
}
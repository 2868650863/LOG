#pragma once
#include <iostream>
#include <thread>
#include <string>
#include "level.hpp"
#include "tool.hpp"

namespace logsys
{
    class logMsg
    {
    public:
        time_t _ctime;          // 产生时间
        logLevel::value _value; // 等级
        size_t _line;           // 行号
        string _file;           // 产生文件
        std::thread::id _tid;   // 线程id
        string _logger;         // 日志器名称
        string _payload;        // 日志内容
    public:
        logMsg(logLevel::value lvalue,
               size_t line,
               const std::string file,
               const std::string logger,
               const std::string payload)
            : _ctime(tool::Date::now()),
              _value(lvalue),
              _line(line),
              _tid(std::this_thread::get_id()),
              _file(file),
              _logger(logger),
              _payload(payload)
        {
        }
    };
}
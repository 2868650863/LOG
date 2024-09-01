#pragma once

namespace logsys
{
    class logLevel
    {
    public:
        enum class value
        {
            UNKNOW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            OFF
        };

        static const char *toString(logLevel::value level)
        {
            switch (level)
            {
            case logLevel::value::DEBUG:
                return "DEBUG";
            case logLevel::value::INFO:
                return "INFO";
            case logLevel::value::WARN:
                return "WARN";
            case logLevel::value::ERROR:
                return "ERROR";
            case logLevel::value::FATAL:
                return "FATAL";
            case logLevel::value::OFF:
                return "OFF";
           
            };
            return "UNKNOW";
        }
    };

}
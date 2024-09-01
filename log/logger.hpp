#pragma once

#include "tool.hpp"
#include "format.hpp"
#include "level.hpp"
#include "outlet.hpp"
#include "message.hpp"
#include <atomic>
#include <mutex>
#include <stdarg.h>
#include "buffer.hpp"
#include "asyncWorker.hpp"

//***************************建造者模式 */

// 日志器模块

namespace logsys
{
    class Logger
    {
    public:
        using ptr_logger = std::shared_ptr<Logger>;
        Logger(const string &logger_name,
               logLevel::value level,
               Formatter::ptr_formater &formater,
               vector<LogOutlet::ptr_outlet> &outlets)
            : _logger_name(logger_name),
              _limit_lever(level),
              _formater(formater),
              _outlets(outlets)
        {
        }
        const string &getname()
        {
            return _logger_name;
        }
        void debug(const string &file, size_t line, const string fmt, ...)
        {
            if (_limit_lever > logLevel::value::DEBUG)
            {
                return;
            }
            char *res;
            va_list ap;
            va_start(ap, fmt);
            int len = vsnprintf(NULL, 0, fmt.c_str(), ap);
            va_end(ap);
            int ret = -1;
            if (len < 0)
            {
                cout << "vsnprintf写入出错: " << __FILE__ << __LINE__ << endl;
                return;
            }
            res = (char *)malloc(len + 1);
            if (res == nullptr)
            {
                cout << "malloc失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            va_start(ap, fmt);
            ret = vsnprintf(res, (len + 1), fmt.c_str(), ap);
            va_end(ap);
            if (ret < 0)
            {
                cout << "vasprintf获取参数包失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            logMsg msg(logLevel::value::DEBUG, line, file, _logger_name, res);
            std::stringstream ss;
            _formater->format(ss, msg);
            log(ss.str().c_str(), ss.str().size());
            free(res);
        }
        void info(const string &file, size_t line, const string fmt, ...)
        {
            if (_limit_lever > logLevel::value::INFO)
            {
                return;
            }
            char *res;
            va_list ap;
            va_start(ap, fmt);
            int len = vsnprintf(NULL, 0, fmt.c_str(), ap);
            va_end(ap);
            int ret = -1;
            if (len < 0)
            {
                cout << "vsnprintf写入出错: " << __FILE__ << __LINE__ << endl;
                return;
            }
            res = (char *)malloc(len + 1);
            if (res == nullptr)
            {
                cout << "malloc失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            va_start(ap, fmt);
            ret = vsnprintf(res, (len + 1), fmt.c_str(), ap);
            va_end(ap);
            if (ret < 0)
            {
                cout << "vasprintf获取参数包失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            logMsg msg(logLevel::value::INFO, line, file, _logger_name, res);
            std::stringstream ss;
            _formater->format(ss, msg);
            log(ss.str().c_str(), ss.str().size());
            free(res);
        }
        void warn(const string &file, size_t line, const string fmt, ...)
        {
            if (_limit_lever > logLevel::value::WARN)
            {
                return;
            }
            char *res;
            va_list ap;
            va_start(ap, fmt);
            int len = vsnprintf(NULL, 0, fmt.c_str(), ap);
            va_end(ap);
            int ret = -1;
            if (len < 0)
            {
                cout << "vsnprintf写入出错: " << __FILE__ << __LINE__ << endl;
                return;
            }
            res = (char *)malloc(len + 1);
            if (res == nullptr)
            {
                cout << "malloc失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            va_start(ap, fmt);
            ret = vsnprintf(res, (len + 1), fmt.c_str(), ap);
            va_end(ap);
            if (ret < 0)
            {
                cout << "vasprintf获取参数包失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            logMsg msg(logLevel::value::WARN, line, file, _logger_name, res);
            std::stringstream ss;
            _formater->format(ss, msg);
            log(ss.str().c_str(), ss.str().size());
            free(res);
        }
        void error(const string &file, size_t line, const string fmt, ...)
        {
            if (_limit_lever > logLevel::value::ERROR)
            {
                return;
            }
            char *res;
            va_list ap;
            va_start(ap, fmt);
            int len = vsnprintf(NULL, 0, fmt.c_str(), ap);
            va_end(ap);
            int ret = -1;
            if (len < 0)
            {
                cout << "vsnprintf写入出错: " << __FILE__ << __LINE__ << endl;
                return;
            }
            res = (char *)malloc(len + 1);
            if (res == nullptr)
            {
                cout << "malloc失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            va_start(ap, fmt);
            ret = vsnprintf(res, (len + 1), fmt.c_str(), ap);
            va_end(ap);
            if (ret < 0)
            {
                cout << "vasprintf获取参数包失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            logMsg msg(logLevel::value::ERROR, line, file, _logger_name, res);
            std::stringstream ss;
            _formater->format(ss, msg);
            log(ss.str().c_str(), ss.str().size());
            free(res);
        }
        void fatal(const string &file, size_t line, const string fmt, ...)
        {
            if (_limit_lever > logLevel::value::FATAL)
            {
                return;
            }
            char *res;
            va_list ap;
            va_start(ap, fmt);
            int len = vsnprintf(NULL, 0, fmt.c_str(), ap);
            va_end(ap);
            int ret = -1;
            if (len < 0)
            {
                cout << "vsnprintf写入出错: " << __FILE__ << __LINE__ << endl;
                return;
            }
            res = (char *)malloc(len + 1);
            if (res == nullptr)
            {
                cout << "malloc失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            va_start(ap, fmt);
            ret = vsnprintf(res, (len + 1), fmt.c_str(), ap);
            va_end(ap);
            if (ret < 0)
            {
                cout << "vasprintf获取参数包失败: " << __FILE__ << __LINE__ << endl;
                return;
            }
            logMsg msg(logLevel::value::FATAL, line, file, _logger_name, res);
            std::stringstream ss;
            _formater->format(ss, msg);
            log(ss.str().c_str(), ss.str().size());
            free(res);
        }

    protected:
        virtual void log(const char *date, size_t len) = 0;

    protected:
        std::mutex _mutex;
        string _logger_name;                       // 日志器名称
        std::atomic<logLevel::value> _limit_lever; // 日志限制等级  原子操作
        Formatter::ptr_formater _formater;         // 格式化
        vector<LogOutlet::ptr_outlet> _outlets;    // 输出方向数组，可以往多个方向输出
    };

    // 异步日志器
    class AsyncLogger : public Logger
    {
    public:
        AsyncLogger(const string &logger_name,
                    logLevel::value level,
                    Formatter::ptr_formater &formater,
                    vector<LogOutlet::ptr_outlet> &outlets,
                    AsyncType async_type)
            : Logger(logger_name, level, formater, outlets), _async(std::make_shared<AsyncWorker>(std::bind(&AsyncLogger::realLog, this, std::placeholders::_1), async_type))
        {
        }
        void log(const char *date, size_t len) override
        {
            _async->push(date, len);
        }
        // 将缓冲区中的数据进行实际输出
        void realLog(Buffer &buf)
        {
            if (_outlets.empty())
            {
                return;
            }
            for (auto &it : _outlets)
            {
                it->log(buf.begin(), buf.readAbleSize());
            }
        }

    private:
        AsyncWorker::ptr_async _async;
    };

    // 同步日志器
    class SyncLogger : public Logger
    {
    public:
        SyncLogger(const string &logger_name,
                   logLevel::value level,
                   Formatter::ptr_formater &formater,
                   vector<LogOutlet::ptr_outlet> &outlets)
            : Logger(logger_name, level, formater, outlets)
        {
        }

    protected:
        // 直接通过落地模块落地
        void log(const char *date, size_t len) override
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_outlets.empty())
            {
                return;
            }
            for (auto &it : _outlets)
            {
                it->log(date, len);
            }
        }
    };

    //*************建造者模式

    // 日志器模式
    enum class LoggerType
    {
        LOGGER_SYNC, // 同步
        LOGGER_ASYNC // 异步
    };

    // 抽象建造者
    class LoggerBuild
    {
    public:
        LoggerBuild()
            : _logger_type(LoggerType::LOGGER_SYNC), _limit_lever(logLevel::value::DEBUG),
              _async_type(AsyncType::ASYNC_SAFE)
        {
        }
        void buildLoggerType(LoggerType type) { _logger_type = type; }
        void buildAsyncUnsafe() { _async_type = AsyncType::ASYNC_UNSAFE; };
        void buildLoggerName(const string &name) { _logger_name = name; }
        void buildLoggerLevel(logLevel::value level) { _limit_lever = level; }
        void buildFormater(const string &patten)
        {
            _formater = std::make_shared<Formatter>(patten);
        }
        template <class OutType, class... Args>
        void buildOutlets(Args &&...args)
        {
            LogOutlet::ptr_outlet pout = outletFactory::create<OutType>(std::forward<Args>(args)...);
            _outlets.push_back(pout);
        }
        virtual Logger::ptr_logger build() = 0;

    protected:
        AsyncType _async_type;                  // 异步日志器模式
        LoggerType _logger_type;                // 日志器模式
        string _logger_name;                    // 日志器名称
        logLevel::value _limit_lever;           // 日志限制等级  原子操作
        Formatter::ptr_formater _formater;      // 格式化
        vector<LogOutlet::ptr_outlet> _outlets; // 输出方向数组，可以往多个方向输出
    };

    // 具体建造者
    class localLoggerBuilder : public LoggerBuild
    {
    public:
        Logger::ptr_logger build() override
        {
            assert(_logger_name.empty() == false);
            if (_formater.get() == nullptr)
            {
                _formater = std::make_shared<Formatter>();
            }
            if (_outlets.empty())
            {
                buildOutlets<StdoutOutlet>();
            }

            if (_logger_type == LoggerType::LOGGER_ASYNC)
            {
                // 异步日志器
                return std::make_shared<AsyncLogger>(_logger_name, _limit_lever, _formater, _outlets, _async_type);
            }
            // 同步日志器
            return std::make_shared<SyncLogger>(_logger_name, _limit_lever, _formater, _outlets);
        }
    };

    class LoggerManager
    {
    public:
        static LoggerManager &getInstance()
        {
            // c++ 11 当静态局部变量没有构造完成之前，其他的线程进入会阻塞
            static LoggerManager eton;
            return eton;
        }
        // 添加日志器
        void addLogger(Logger::ptr_logger &logger)
        {
            if(hasLogger(logger->getname()))
            {
                return;
            }
            std::unique_lock<std::mutex> lock(_mtx);
            _loggers.insert(std::make_pair(logger->getname(),logger));
        }
        // 查看日志器是否在数组中
        bool hasLogger(const string &name)
        {
            std::unique_lock<std::mutex> lock(_mtx);
            auto it=_loggers.find(name);
            if(it==_loggers.end())
            {
                return false;
            }
            return true;
        }
        // 获取日志器
        Logger::ptr_logger getLogger(const string &name)
        {
            std::unique_lock<std::mutex> lock(_mtx);
            auto it = _loggers.find(name);
            if (it == _loggers.end())
            {
                return Logger::ptr_logger();
            }
            return it->second;
        }
        // 获取默认日志器
        Logger::ptr_logger rootLogger()
        {
            return _root_logger;
        }

    private:
        LoggerManager()
         {
            std::unique_ptr<LoggerBuild> builder(new localLoggerBuilder());
            builder->buildLoggerName("root");
            _root_logger=builder->build();
            _loggers.insert(std::make_pair("root",_root_logger));
         }

    private:
        std::mutex _mtx;
        Logger::ptr_logger _root_logger; // 默认日志器
        std::unordered_map<string, Logger::ptr_logger> _loggers;
    };

    //全局日志器建造者
    class GrobalLoggerBuilder : public LoggerBuild
    {
    public:
        Logger::ptr_logger build() override
        {
            assert(_logger_name.empty() == false);
            if (_formater.get() == nullptr)
            {
                _formater = std::make_shared<Formatter>();
            }
            if (_outlets.empty())
            {
                buildOutlets<StdoutOutlet>();
            }
            Logger::ptr_logger logger;
            if (_logger_type == LoggerType::LOGGER_ASYNC)
            {
                // 异步日志器
                logger= std::make_shared<AsyncLogger>(_logger_name, _limit_lever, _formater, _outlets, _async_type);
            }
            else
            {
                // 同步日志器
                logger= std::make_shared<SyncLogger>(_logger_name, _limit_lever, _formater, _outlets);
            }
            LoggerManager::getInstance().addLogger(logger);
            return logger;
           
        }
    };
}
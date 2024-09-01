#pragma once
#include <memory>
#include <fstream>
#include <assert.h>
#include <sstream>
#include "tool.hpp"
#include <thread>
#include <mutex>

//*******************************简单工厂模式

#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#else
#define PLATFORM_POSIX
#endif
// 日志输出方向
namespace logsys
{
    // 抽象父类
    class LogOutlet
    {
    public:
        LogOutlet() {}
        virtual ~LogOutlet() {} // 子类有需要就重写
        using ptr_outlet = std::shared_ptr<LogOutlet>;
        virtual void log(const char *date, size_t len) = 0;
    };

    // 派生子类，实现输出方向
    // 标准输出流
    class StdoutOutlet : public LogOutlet
    {
    public:
        void log(const char *date, size_t len) override
        {
            cout.write(date, len);
        }
    };

    // 文件流  不限制大小
    class FileOutlet : public LogOutlet
    {
    public:
        FileOutlet(const string &pfilename) : _pfilename(pfilename)
        {
            tool::File::createDirectory(tool::File::path(pfilename)); // 创建路径
            _ofs.open(_pfilename, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        void log(const char *date, size_t len) override
        {
            _ofs.write(date, len);
            assert(_ofs.good());
        }

    private:
        string _pfilename; // 带路径的文件名
        std::ofstream _ofs;
    };

    // 滚动文件
    // 每个日志文件保存固定大小
    class RollFileBySizeOutlet : public LogOutlet
    {
    public:
        RollFileBySizeOutlet(const string &basename, size_t max_fsize)
            : _basename(basename), _max_fsize(max_fsize), _cur_fsize(0)
        {

            string pfilename = createNewFile();                       // 构建日志文件名
            tool::File::createDirectory(tool::File::path(pfilename)); // 创建文件目录
            _ofs.open(pfilename, std::ios::binary | std::ios::app);   // 创建并打开文件
            // cout << pfilename << endl;
            assert(_ofs.is_open());
        }
        void log(const char *date, size_t len) override
        {
            if (_cur_fsize + len > _max_fsize)
            {
                _ofs.close();
                assert(_ofs.good());
                string pfilename = createNewFile();                     // 构建日志文件名
     // tool::File::createDirectory(pfilename);                 // 创建文件目录
                _ofs.open(pfilename, std::ios::binary | std::ios::app); // 创建并打开文件
                assert(_ofs.is_open());
                _cur_fsize = 0;
            }
            _ofs.write(date, len);
            _cur_fsize += len;
            assert(_ofs.good());
        }

    private:
        // 判断当前文件大小是否需要创建新日志文件
        string createNewFile()
        {
            time_t now = tool::Date::now();
            struct tm result;
            std::stringstream filename;
            filename << _basename;
#ifdef PLATFORM_WINDOWS
            // Windows 平台使用 localtime_s
            localtime_s(&result, &now);
#elif defined(PLATFORM_POSIX)
            // POSIX 平台使用 localtime_r
            localtime_r(&now, &result);
#else
            // 如果不支持上述平台，则返回 false
            return false;
#endif
            filename << result.tm_year + 1900 << "-"
                     << result.tm_mon + 1 << "-"
                     << result.tm_mday << "_"
                     << result.tm_hour << "."
                     << result.tm_min << "."
                     << result.tm_sec
                     << "-" << cntname++
                     << ".log";
            return filename.str();
        }

    private:
        size_t cntname = 0; // 文件区分后缀
        string _basename;   // 通用文件名+区分标志（时间）生成日志文件名
        std::ofstream _ofs; // 管理打开的文件
        size_t _max_fsize;  // 日志文件的大小
        size_t _cur_fsize;  // 当前打开文件写入的数据大小
    };

    enum class TimeUnit
    {
        SECOND,
        MINUTE,
        HOUR,
        DAY
    };
    class RollFileByTimeOutlet : public LogOutlet
    {
    public:
        RollFileByTimeOutlet(const string &basename, TimeUnit unit, size_t time_num)
        :_basename(basename)
        {
            string pfilename = createNewFile();                       // 构建日志文件名
            tool::File::createDirectory(tool::File::path(pfilename)); // 创建文件目录
            _ofs.open(pfilename, std::ios::binary | std::ios::app);   // 创建并打开文件
            // cout << pfilename << endl;
            assert(_ofs.is_open());
            switch (unit)
            {
            case TimeUnit::SECOND:
                _second_num = time_num;
                break;
            case TimeUnit::MINUTE:
                _second_num = 60 * time_num;
                break;
            case TimeUnit::HOUR:
                _second_num = 60 * 60 * time_num;
                break;
            case TimeUnit::DAY:
                _second_num = 60 * 60 * 24 * time_num;
                break;
            default:
                cout << "时间单位错误" << endl;
                break;
            }
            _cur_time = time(nullptr);
            _cur_gap=tool::Date::now()%_second_num;   
        }
     
        void log(const char *date, size_t len) override
        {
            //if(tool::Date::now%_second_num%!=_cur_gap)
            if (tool::Date::now() - _cur_time >= _second_num)
            {
                _ofs.close();
                assert(_ofs.good());
                string pfilename = createNewFile(); // 构建日志文件名
                _ofs.open(pfilename,std::ios::binary|std::ios::app);
                assert(_ofs.is_open());
                _cur_time = tool::Date::now(); // 更新当前时间
            }
            _ofs.write(date, len);
            assert(_ofs.good());
        }

    private:
        // 判断当前文件大小是否需要创建新日志文件
        string createNewFile()
        {
            time_t now = tool::Date::now();
            struct tm result;
            std::stringstream filename;
            filename << _basename;
#ifdef PLATFORM_WINDOWS
            // Windows 平台使用 localtime_s
            localtime_s(&result, &now);
#elif defined(PLATFORM_POSIX)
            // POSIX 平台使用 localtime_r
            localtime_r(&now, &result);
#else
            // 如果不支持上述平台，则返回 false
            return false;
#endif
            filename << result.tm_year + 1900 << "-"
                     << result.tm_mon + 1 << "-"
                     << result.tm_mday << "."
                     << result.tm_hour << "-"
                     << result.tm_min << "-"
                     << result.tm_sec
                     << "-" << cntname++
                     << ".log";
            return filename.str();
        }

    private:
        size_t cntname = 0; // 文件区分后缀
        string _basename;   // 通用文件名+区分标志（时间）生成日志文件名
        std::ofstream _ofs; // 管理打开的文件
        size_t _second_num; // 时间秒数  间隔时间
        size_t _cur_time;   // 当前时间
        size_t _cur_gap;
    };

    // 日志类生产工厂
    class outletFactory
    {
    public:
        template <class OutletType, class... Args>
        static LogOutlet::ptr_outlet create(Args &&...args)
        {
            return std::make_shared<OutletType>(std::forward<Args>(args)...);
        }
    };
}
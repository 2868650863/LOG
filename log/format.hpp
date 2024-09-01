#pragma once
#include "level.hpp"
#include "message.hpp"
#include <ctime>
#include <vector>
#include <cassert>
#include <sstream>

//*******************工厂方法模式

#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)
#define PLATFORM_POSIX
#endif

using std::vector;

namespace logsys
{
    // 抽象父类
    class FormatItem
    {
    public:
        using ptr_format = std::shared_ptr<FormatItem>;
        virtual void format(std::ostream &out, const logMsg &msg) = 0;
    };

    // 派生子类
    // 日志内容
    class MsgFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << msg._payload;
        }
    };

    // 日志等级
    class LevelFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << logLevel::toString(msg._value);
        }
    };

    // 时间格式
    class TimeFormatItem : public FormatItem
    {
    public:
        TimeFormatItem(const string &fmt = "%H:%M:%S") : _time_fmt(fmt)
        {
        }
        void format(std::ostream &out, const logMsg &msg) override
        {
            struct tm t;
#ifdef PLATFORM_WINDOWS
            // Windows 平台使用 localtime_s
            localtime_s(&t, &msg._ctime);
#elif defined(PLATFORM_POSIX)
            // POSIX 平台使用 localtime_r
            localtime_r(&msg._ctime, &t);
#else
            return;
#endif
            // time_t nowtime = msg._ctime;
            // t = localtime(&nowtime);
            // localtime_r(&msg._ctime, &t);
            char buffer[64] = {0};
            strftime(buffer, 63, _time_fmt.c_str(), &t);
            out << buffer;
        }

    private:
        string _time_fmt; // 时间格式字符串
    };
    // 文件名
    class FileFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << msg._file;
        }
    };
    // 行号
    class LineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << msg._line;
        }
    };
    // 线程ID
    class ThreadFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << msg._tid;
        }
    };
    // 日志器名称
    class LoggerFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << msg._logger;
        }
    };
    // 制表符
    class TabFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << "\t";
        }
    };
    // 换行
    class NewlineFormatItem : public FormatItem
    {
    public:
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << "\n";
        }
    };
    // 其他字符
    class OthreFormatItem : public FormatItem
    {
    public:
        OthreFormatItem(const string &str) : _str(str)
        {
        }
        void format(std::ostream &out, const logMsg &msg) override
        {
            out << _str;
        }

    private:
        string _str;
    };

    // %d ⽇期
    // %T 缩进
    // %t 线程id
    // %p ⽇志级别
    // %c ⽇志器名称
    // %f ⽂件名
    // %l ⾏号
    // %m ⽇志消息
    // %n 换⾏

    // 字符串解析
    // string s="[%d{%H:%M:%S}][%t][%p][%c][%f:%l] %m%n";

    // 字符串格式化工厂
    class Formatter
    {
    public:
        using ptr_formater = std::shared_ptr<Formatter>;
        Formatter(const string &patten = "[%p][%f:%l][%d{%H:%M:%S}][%t][%c]%T%m%n")
            : _patten(patten)
        {
            assert(parsePatten());
        }

        // 对msg内容格式化
        void format(std::ostream &out, const logMsg &msg)
        {

            for (auto &it : _items)
            {

                it->format(out, msg);
            }
        }
        string format(const logMsg &msg)
        {
            std::stringstream ss;
            format(ss, msg);
            return ss.str();
        }

    private:
        // 对格式化的字符串解析
        bool parsePatten()
        {
            vector<std::pair<string, string>> kv;
            if (_patten.size() == 0)
            {
                // 空串
                kv.push_back(std::make_pair("", ""));
                return true;
            }

            int pos = 0;
            string key, value;
            while (pos < _patten.size())
            {

                if (_patten[pos] != '%')
                {
                    // 普通字符串
                    value.push_back(_patten[pos]);
                    pos++;
                    continue;
                }
                // 是%%或者%%%...
                if (pos + 1 < _patten.size() && _patten[pos + 1] == '%')
                {
                    // 至少两个%以上
                    int len = pos;
                    while (pos + 1 < _patten.size() && _patten[pos + 1] == '%')
                    {
                        pos++;
                    }
                    len = pos - len + 1;
                    if (len % 2 == 0)
                    {
                        // 只输出%
                        len /= 2;
                        while (len--)
                        {
                            value += "%";
                        }
                        ++pos;
                    }
                    else
                    {
                        // 奇数个%，后面有格式字符
                        len -= 1;
                        len /= 2;
                        while (len--)
                        {
                            value += "%";
                        }
                    }

                    continue;
                }
                if (!value.empty())
                {
                    kv.push_back(std::make_pair("", value));
                    value.clear();
                }

                // % + 字符
                ++pos;
                if (pos >= _patten.size())
                {
                    cout << "%后没有格式化字符" << endl;
                    return false;
                }
                key = _patten[pos];
                ++pos;

                if (pos < _patten.size() && _patten[pos] == '{')
                {

                    ++pos;
                    while (pos < _patten.size() && _patten[pos] != '}')
                    {
                        value.push_back(_patten[pos]);
                        ++pos;
                    }
                    if (pos == _patten.size())
                    {
                        cout << "{}匹配出错.." << endl;
                        return false;
                    }
                    pos += 1;
                }

                kv.push_back(std::make_pair(key, value));
                key.clear();
                value.clear();
            }

            for (auto &it : kv)
            {
                // cout<<it.first<<"--"<<it.second<<endl;
                _items.push_back(createItem(it.first, it.second));
            }
            return true;
        }

        // 根据不同字符创建不容子类
        FormatItem::ptr_format createItem(const string &key, const string &value)
        {

            // key  格式
            // value  time，other派生类参数

            if (key == "t")
                return std::make_shared<ThreadFormatItem>();
            if (key == "T")
                return std::make_shared<TabFormatItem>();
            if (key == "p")
                return std::make_shared<LevelFormatItem>();
            if (key == "c")
                return std::make_shared<LoggerFormatItem>();
            if (key == "f")
                return std::make_shared<FileFormatItem>();
            if (key == "l")
                return std::make_shared<LineFormatItem>();
            if (key == "m")
                return std::make_shared<MsgFormatItem>();
            if (key == "n")
                return std::make_shared<NewlineFormatItem>();
            if (key == "d")
                return std::make_shared<TimeFormatItem>(value);
            if (key.empty())
                return std::make_shared<OthreFormatItem>(value);
            cout << "没有对应的格式操作 %" << key << endl;
            exit(-1);
            return FormatItem::ptr_format();
        }

    private:
        string _patten;                             // 格式化字符串
        std::vector<FormatItem::ptr_format> _items; // 父类指针指向子类对象数组
    };
}
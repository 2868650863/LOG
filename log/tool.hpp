#pragma once
#include <iostream>
#include <ctime>
#include <filesystem>
#include <sys/stat.h>
#include <assert.h>
#include <cstdlib>

//工具

using std::cout;
using std::endl;
using std::string;


namespace logsys
{
    namespace tool
    {
        class Date
        {
        public:
            static size_t now()
            {
                return (size_t)time(nullptr);
            }
        };
        class File
        {
            public:
                static bool exists(const string &pathname)  // 判断是否存在
                {
                    struct stat st;
                    if(stat(pathname.c_str(),&st)<0)
                    {
                        //获取文件属性失败,文件不存在
                        return false;
                    }
                    return true;
                }

                static string path(const string &pathname)  // 文件路径
                {
                    size_t pos=pathname.find_last_of("/\\");
                    if(pos==string::npos)
                    {
                        return ".";
                    }
                    return pathname.substr(0,pos+1);
                }
                static void createDirectory(const string &pathname)// 创建目录文件
                {
                    //  /a/b/
                    std::filesystem::create_directories(pathname.c_str());
                }
        };
    }
}
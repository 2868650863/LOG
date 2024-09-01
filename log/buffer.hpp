#pragma once
#include "tool.hpp"
#include <vector>
#include <string>
#include <assert.h>

//异步日志器中的缓冲区
using std::string;
using std::vector;

namespace logsys
{
#define BUFFER_SIZE (1024 * 1024 * 1)     //
#define THRESHOUD_SIZE (1024 * 1024 * 10) // 扩容阈值
#define INCREAT_SIZE (1024 * 1024 * 1)    // 线性增长
    class Buffer
    {
    public:
        Buffer() : _buffer(BUFFER_SIZE), _read_idx(0), _write_idx(0)
        {
        }
        void push(const char *date, size_t len) // 向缓冲区中写入数据
        {
            // if(len>writeAbleSize()) return;
            ensureEnoughSize(len);
            std::copy(date, date + len, &_buffer[_write_idx]);
            moveWritePtr(len);
        }
        // 可读数据起始地址
        const char *begin()
        {
            return &_buffer[_read_idx];
        }
        size_t readAbleSize() // 返回可读数据大小
        {
            return (_write_idx - _read_idx);
        }
        size_t writeAbleSize() // 返回可写数据大小
        {
            // 对于固定数据大小的缓冲区
            return (_buffer.size() - _write_idx - 1);
        }
        // 移动读指针
        void moveReadPtr(size_t len)
        {
            assert(len <= readAbleSize());
            _read_idx += len;
        }

        // 重置读写位置
        void reset()
        {
            _read_idx = 0;
            _write_idx = 0;
        }

        void swap(Buffer &other)
        {
            _buffer.swap(other._buffer);
            std::swap(_read_idx, other._read_idx);
            std::swap(_write_idx, other._write_idx);
        }
        bool empty()
        {
            return _read_idx == _write_idx;
        }

    private:
        void ensureEnoughSize(size_t len) // 扩容
        {
            if (len <= writeAbleSize())
                return;
            size_t newsize = 0;
            if (_buffer.size() < THRESHOUD_SIZE)
            {
                // 当前容量小于扩容阈值
                newsize = _buffer.size() * 2 + len; // 2倍增长
            }
            else
            {
                newsize = _buffer.size() + INCREAT_SIZE + len; // 线性增长
            }
            cout << "扩容： " << newsize << endl;
            _buffer.resize(newsize);
        }

        void moveWritePtr(size_t len)
        {
            assert(len + _write_idx < _buffer.size());
            _write_idx += len;
        }

    private:
        vector<char> _buffer;
        size_t _read_idx;  // 读取位置
        size_t _write_idx; // 写入位置
    };
}
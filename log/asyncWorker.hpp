#pragma once
#include "buffer.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <atomic>


namespace logsys
{
    using func = std::function<void(Buffer &)>;
    enum class AsyncType
    {
        ASYNC_SAFE,  // 安全模式，缓冲区满了就阻塞
        ASYNC_UNSAFE // 非安全模式缓冲区满了就扩容
    };
    class AsyncWorker
    {
    public:
        using ptr_async = std::shared_ptr<AsyncWorker>;
        AsyncWorker(const func handler, AsyncType async_type = AsyncType::ASYNC_SAFE)
            : _stop(false),
              _async_topy(AsyncType::ASYNC_SAFE),
              _t(std::thread(&AsyncWorker::threadEntry, this)),
              _callback(handler)
        {
        }
        ~AsyncWorker()
        {
            stop();
        }
        void stop()
        {
            _stop = true;
            _cond_con.notify_all(); // 唤醒所有工作线程
            _t.join();              // 等待线程退出
        }
        void push(const char *date, size_t len)
        {
            std::unique_lock<std::mutex> lock(_mtx);
            if (_async_topy == AsyncType::ASYNC_SAFE)
            {
                _cond_pro.wait(lock, [&]()
                               { return _pro_buf.writeAbleSize() >= len; });
            }
               
            _pro_buf.push(date, len);
            _cond_con.notify_one(); // 唤醒一个线程消费
        }

    private:
        void threadEntry() // 线程入口函数
        {

            while (1)
            {
                {
                    std::unique_lock<std::mutex> lock(_mtx);
                    if (_stop && _pro_buf.empty())
                    {
                        // 防止生产缓冲区中还有数据
                        break;
                    }
                    _cond_con.wait(lock, [&]()
                                   { return _stop || !_pro_buf.empty(); }); // 没有数据就等待
                  
                    _con_buf.swap(_pro_buf);
                    if (_async_topy == AsyncType::ASYNC_SAFE)
                    {
                        //安全模式才需要唤醒
                        _cond_pro.notify_all(); // 唤醒生产线程
                    }
                      
                }
                _callback(_con_buf); // 处理消费数据
                _con_buf.reset();
            }
        }
        private:
        func _callback; // 线程回调函数
    private:
        AsyncType _async_topy;    //选择缓冲区工作模式
        std::atomic<bool> _stop; // 停止工作
        Buffer _pro_buf;         // 生产者缓冲区
        Buffer _con_buf;         // 消费者缓冲区
        std::mutex _mtx;
        std::condition_variable _cond_pro; // 生产者条件变量
        std::condition_variable _cond_con; // 消费者条件变量
        std::thread _t;                    // 异步工作线程
    };
}
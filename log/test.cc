#include "log.hpp"

using namespace logsys;
// %d ⽇期
// %T 缩进
// %t 线程id
// %p ⽇志级别
// %c ⽇志器名称
// %f ⽂件名
// %l ⾏号
// %m ⽇志消息
// %n 换⾏

void testlog(const string &name)
{
    Logger::ptr_logger logger = LoggerManager::getInstance().getLogger("sync_logger");
    string str = "--测试日志";
    FATAL("%s", "测试开始");
    logger->debug("%s", str.c_str());
    logger->info("%s", str.c_str());
    logger->warn("%s", str.c_str());
    logger->error("%s", str.c_str());
    logger->fatal("%s", str.c_str());
    FATAL("%s", "测试结束");

    //  int cnt = 0;
    //  while (cnt < 50000)
    //  {
    //      logger->fatal( "测试日志-%d", ++cnt);
    //  }
    // FATAL("%s", "测试开始");
    // string str = "--测试日志";
    // DEBUG("%s", str.c_str());
    // INFO("%s", str.c_str());
    // WARN("%s", str.c_str());
    // ERROR("%s", str.c_str());
    // FATAL("%s", str.c_str());
    // FATAL("%s", "测试结束");
}

string s = "[%d{%H:%M:%S}][%t][%p][%c][%f:%l] %m%n";
int main()
{

    std::unique_ptr<LoggerBuild> builder(new GrobalLoggerBuilder());
    builder->buildLoggerName("sync_logger");
    builder->buildLoggerLevel(logLevel::value::DEBUG);
    builder->buildFormater("[%f:%l][%p][%c][%d{%H:%M:%S}]   %m%n");
    builder->buildLoggerType(LoggerType::LOGGER_SYNC);
    // builder->buildAsyncUnsafe();
    builder->buildOutlets<StdoutOutlet>();
    builder->buildOutlets<FileOutlet>("./logfile/async.log");
    // builder->buildOutlets<RollFileBySizeOutlet>("./logfile/roll-async-size-", 1024 * 1024);
    // builder->buildOutlets<RollFileByTimeOutlet>("./logfile/roll-async-time-", TimeUnit::SECOND,5 );
    builder->build();
    testlog("async_logger");
     Logger::ptr_logger logger = LoggerManager::getInstance().rootLogger();
    // logger->fatal("%s","aaaa");

    return 0;
}
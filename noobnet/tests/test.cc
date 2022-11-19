#include "../net/log.h"
#include <iostream>

int main(int argc, const char** argv) {
    noobnet::Logger::ptr logger(new noobnet::Logger);
    logger->addAppender(noobnet::LogAppender::ptr(new noobnet::StdoutLogAppender));

    noobnet::LogEvent::ptr event(new noobnet::LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));
    
    logger->log(noobnet::LogLevel::DEBUG, event);
    std::cout << "hello mylog" << std::endl;

    return 0;
}
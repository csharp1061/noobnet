#ifndef _LOG_
#define _LOG_
#define APPDENER_NUM 5

#include "singleton.h"

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>


namespace noobnet {

#define SYS_LOG_LEVEL(logger, level) \
  if (logger->getLevel() <= level)  \
    noobnet::LogEventWrap(noobnet::LogEvent::ptr\
    (new noobnet::LogEvent(__FILE__, __LINE__,\
    0, 1, 2, time(0), logger, level))).getSS()


#define SYS_LOG_DEBUG(logger) SYS_LOG_LEVEL(logger, noobnet::LogLevel::DEBUG)

#define SYS_LOG_WARN(logger) SYS_LOG_LEVEL(logger, noobnet::LogLevel::WARN)

#define SYS_LOG_ERROR(logger) SYS_LOG_LEVEL(logger, noobnet::LogLevel::ERROR)

#define SYS_LOG_INFO(logger) SYS_LOG_LEVEL(logger, noobnet::LogLevel::INFO)

#define SYS_LOG_FATAL(logger) SYS_LOG_LEVEL(logger, noobnet::LogLevel::FATAL)

//自定义输出格式
#define SYS_LOG_FMT_LEVEL(logger, level, fmt, ...) \
  if (logger->getLevel() <= level)  \
    noobnet::LogEventWrap(noobnet::LogEvent::ptr\
    (new noobnet::LogEvent(__FILE__, __LINE__,\
    0, 1, 2, time(0), logger, level))).getEvent()->format(fmt, __VA_ARGS__)

#define SYS_LOG_FMT_DEBUG(logger, fmt, ...) SYS_LOG_FMT_LEVEL(logger, noobnet::LogLevel::DEBUG, fmt, __VA_ARGS__)

#define SYS_LOG_FMT_WARN(logger, fmt, ...) SYS_LOG_FMT_LEVEL(logger, noobnet::LogLevel::WARN, fmt, __VA_ARGS__)

#define SYS_LOG_FMT_ERROR(logger, fmt, ...) SYS_LOG_FMT_LEVEL(logger, noobnet::LogLevel::ERROR, fmt, __VA_ARGS__)

#define SYS_LOG_FMT_INFO(logger, fmt, ...) SYS_LOG_FMT_LEVEL(logger, noobnet::LogLevel::INFO, fmt, __VA_ARGS__)

#define SYS_LOG_FMT_FATAL(logger, fmt, ...) SYS_LOG_FMT_LEVEL(logger, noobnet::LogLevel::FATAL, fmt, __VA_ARGS__)

#define SYS_LOG_ROOT() noobnet::LoggerMgr::getInstance()->getRoot()
//前向声明
class LoggerManager;
class Logger;

//日志级别
class LogLevel {
 public:
  enum level {
    UNKOWN = 0,
    DEBUG = 1,
    WARN = 2,
    ERROR = 3,
    INFO = 4,
    FATAL = 5
  };

    static const char* ToString(LogLevel::level level);
};

//日志的所有出现的字段由这个类持有,用来表示日志事件
class LogEvent {
 public:
  typedef std::shared_ptr<LogEvent> ptr;
  LogEvent(const char* file, int32_t line, uint32_t elapse, 
            uint32_t thread, uint32_t fiber, uint64_t time,
            std::shared_ptr<Logger> logger, LogLevel::level level);

  const char* getFile() const { return m_file; }
  int32_t getLineID() const { return m_line; }
  uint32_t getThreadID() const { return m_threadID; }
  uint32_t getFiberID() const { return m_fiberID; }
  uint32_t getElapse() const { return m_elapse; }
  uint64_t getTime() const { return m_time; }
  const std::string getContent() const { return m_ss.str(); }
  std::stringstream& getSS() { return m_ss; }
  std::shared_ptr<Logger> getLogger() const { return m_logger; }
  LogLevel::level getLevel() const { return m_level; }

  void format(const char* fmt, ...);
  void format(const char* fmt, va_list al);

 private:
    //文件路径，行号，协程号，线程号，时间，文本
  const char* m_file = nullptr;
  int32_t m_line = 0;
  uint32_t m_elapse = 0; //启动至今的时间间隔
  uint32_t m_threadID = 0;
  uint32_t m_fiberID = 0;
  uint64_t m_time = 0;
  std::stringstream m_ss;
  std::shared_ptr<Logger> m_logger;
  LogLevel::level m_level;
};

class LogEventWrap {
public:
  LogEventWrap(LogEvent::ptr val);
  ~LogEventWrap();

  LogEvent::ptr getEvent() { return m_event; }
  std::stringstream& getSS();
private:
  LogEvent::ptr m_event;
};

//定义输出的格式
class LogFormater {
 public:
  typedef std::shared_ptr<LogFormater> ptr;
  LogFormater(const std::string& pattern);

    //处理为时-分-秒-线程ID-协程ID类似
  std::string format(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event);

    //不同类日志格式的总控制类，之后的其他所有格式都继承自该类
  class FormaterItem {
   public:
    typedef std::shared_ptr<FormaterItem> ptr;
    virtual ~FormaterItem() {};
    virtual void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) = 0;
  };

  void init();
 private:
  std::string m_pattern;
  std::vector<FormaterItem::ptr> m_items;
};

//日志输出地
class LogAppender {
  friend class Logger;
 public:
  typedef std::shared_ptr<LogAppender> ptr;
  virtual ~LogAppender() {};

  virtual void log(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) = 0; 

  void setFormater(LogFormater::ptr val) { m_formater = val; }
  LogFormater::ptr getFormater() const { return m_formater; }

  void setFormater(LogLevel::level val) { m_level = val; }
  LogLevel::level getLevel() const { return m_level; }
 protected:
  LogLevel::level m_level;
  LogFormater::ptr m_formater;
};

//日志类
class Logger : public std::enable_shared_from_this<Logger> {
friend class LoggerManager;
 public:
  typedef std::shared_ptr<Logger> ptr;

  Logger(const std::string name = "root");
  void log(LogLevel::level level, LogEvent::ptr event);

  void debug(ptr logger, LogEvent::ptr event);
  void warn(ptr logger, LogEvent::ptr event);
  void error(ptr logger, LogEvent::ptr event);
  void info(ptr logger, LogEvent::ptr event);
  void fatal(ptr logger, LogEvent::ptr event);

  void addAppender(LogAppender::ptr appender);
  void delAppender(LogAppender::ptr appender);

  LogLevel::level getLevel() { return m_level; }
  void setLevel(LogLevel::level val) { m_level = val; }
  const std::string& getName() const { return m_name; };

 private:
  Logger::ptr m_root;
  std::string m_name;
  LogLevel::level m_level;
  std::list<LogAppender::ptr> m_appenders;  //用链表维护appenders
  LogFormater::ptr m_formater;  //logger也需要一个formater  可能appender直接输出日志
};

//终端日志类
class StdoutLogAppender : public LogAppender {
 public:
  typedef std::shared_ptr<StdoutLogAppender> ptr;
  void log(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) override;
};

//文件日志类
class FileLogAppender : public LogAppender {
 public:
  FileLogAppender(const std::string& filename);

  typedef std::shared_ptr<FileLogAppender> ptr;
  void log(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) override;
  bool reopen(const std::string& filename);
 
 private:
  std::string m_filename;
  std::ofstream m_filestream;
};

//日志管理
class LoggerManager {
public:
  LoggerManager();

  Logger::ptr getLogger(const std::string& name);
  void init();

  Logger::ptr getRoot() const { return m_root; }
private:
  std::map<std::string, Logger::ptr> m_loggers;
  Logger::ptr m_root;
};

typedef noobnet::SingleTon<LoggerManager> LoggerMgr;

} // namespace noobnet


#endif // !_LOG_

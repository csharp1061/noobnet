#ifndef _LOG_
#define _LOG_
#define APPDENER_NUM 5

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>


namespace noobnet {

//前向声明
class Logger;
//日志的所有出现的字段由这个类持有,用来表示日志事件
class LogEvent {
 public:
  typedef std::shared_ptr<LogEvent> ptr;
  LogEvent(const char* file, int32_t line, uint32_t elapse, 
            uint32_t thread, uint32_t fiber, uint64_t time);

  const char* getFile() const { return m_file; }
  int32_t getLineID() const { return m_line; }
  uint32_t getThreadID() const { return m_threadID; }
  uint32_t getFiberID() const { return m_fiberID; }
  uint32_t getElapse() const { return m_elapse; }
  uint64_t getTime() const { return m_time; }
  const std::string getContent() const { return m_ss.str(); }
  const std::stringstream& getSs() const { return m_ss; }

 private:
    //文件路径，行号，协程号，线程号，时间，文本
  const char* m_file = nullptr;
  int32_t m_line = 0;
  uint32_t m_elapse = 0; //启动至今的时间间隔
  uint32_t m_threadID = 0;
  uint32_t m_fiberID = 0;
  uint64_t m_time = 0;
  std::stringstream m_ss;
};

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

//定义输出的格式
class LogFormater {
 public:
  typedef std::shared_ptr<LogFormater> ptr;
  LogFormater(const std::string& pattern);

    //处理为时-分-秒-线程ID-协程ID类似
  std::string format(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event);

    //子模块 负责解析event中的信息
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
 protected:
  LogLevel::level m_level;
  LogFormater::ptr m_formater;
};

//日志类
class Logger : public std::enable_shared_from_this<Logger> {
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
} // namespace noobnet


#endif // !_LOG_

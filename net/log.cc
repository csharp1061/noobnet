#include "log.h"
#include "config.h"

#include <map>
#include <functional>
#include <stdarg.h>
#include <iostream>


namespace noobnet {

//此处不用再次声明static
const char* LogLevel::ToString(LogLevel::level level) {
    switch (level) {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;

    XX(DEBUG);
    XX(ERROR);
    XX(INFO);
    XX(WARN);
    XX(FATAL);
#undef XX
    default:
        return "UNKOWN";
    }
    return "UNKOWN";
}

LogLevel::level LogLevel::FromString(const std::string& str) {
#define XX(level, v) \
    if (str == #v) { \
        return LogLevel::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
    return LogLevel::UNKOWN;
#undef XX
}

LogEventWrap::LogEventWrap(LogEvent::ptr val) : m_event(val) {}

LogEventWrap::~LogEventWrap() {
    m_event->getLogger()->log(m_event->getLevel(), m_event);
}

void LogEvent::format(const char* fmt, ...) {
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char* fmt, va_list al) {
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if (len != -1) {
        m_ss << std::string(buf, len);
        free(buf);
    }
}

std::stringstream& LogEventWrap::getSS() {
    return m_event->getSS();
}

class MessegeFormatItem : public LogFormatter::FormatterItem {
 public:
    MessegeFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getContent();
    }
};
 
class LevelFormatItem : public LogFormatter::FormatterItem {
 public:
    LevelFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << LogLevel::ToString(level);
    }
};

class ElapseFormatItem : public LogFormatter::FormatterItem {
 public:
    ElapseFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getElapse();
    }
};

class NameFormatItem : public LogFormatter::FormatterItem {
 public:
    NameFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << logger->getName();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatterItem {
 public:
    ThreadIdFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getThreadID();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatterItem {
 public:
    FiberIdFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getFiberID();
    }
};

class DateTimeFormatItem : public LogFormatter::FormatterItem {
 public:
    DateTimeFormatItem(const std::string& format = "%Y-%M-%D %H:%M:%S") : m_format(format) {
        if (m_format.empty()) {
            m_format = "%Y-%M-%D %H:%M:%S";
        }
    }
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << event->getTime();
    }
 private:
    std::string m_format;
};

class LineFormatItem : public LogFormatter::FormatterItem {
 public:
    LineFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getLineID();
    }
};

class NextLineFormatItem : public LogFormatter::FormatterItem {
 public:
    NextLineFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class FileNameFormatItem : public LogFormatter::FormatterItem {
 public:
    FileNameFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getFile();
    }
};

class TabFormatItem : public LogFormatter::FormatterItem {
 public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << "\t";
    }
};

class StirngFormatItem : public LogFormatter::FormatterItem {
 public:
    StirngFormatItem(const std::string& fmt) : m_string(fmt) {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << m_string;
    }
 private:
    std::string m_string;
};

LogEvent::LogEvent(const char* file, int32_t line, uint32_t elapse, 
            uint32_t thread, uint32_t fiber, uint64_t time,
            std::shared_ptr<Logger> logger, LogLevel::level level) 
            :m_file(file),
             m_line(line),
             m_elapse(elapse),
             m_threadID(thread),
             m_fiberID(fiber),
             m_time(time),
             m_logger(logger),
             m_level(level) {

}

Logger::Logger(const std::string name) : m_name(name) , m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormatter("%d%T[%p]%T%c%T%m%T%n"));
}

//TODO 看
void Logger::log(LogLevel::level level, LogEvent::ptr event) {
    if (level >= m_level) {
        auto self = shared_from_this();
        if (!m_appenders.empty()) {
            for (auto &i : m_appenders) {
                i->log(self , level, event);
            } 
        } else if (m_root) {
            m_root->log(level, event);
        }
    }
}

void Logger::debug(Logger::ptr logger, LogEvent::ptr event) {
    log(LogLevel::DEBUG, event);
}

void Logger::warn(Logger::ptr logger, LogEvent::ptr event) {
    log(LogLevel::WARN, event);
}

void Logger::error(Logger::ptr logger, LogEvent::ptr event) {
    log(LogLevel::ERROR, event);
}

void Logger::info(Logger::ptr logger, LogEvent::ptr event) {
    log(LogLevel::INFO, event);
}

void Logger::fatal(Logger::ptr logger, LogEvent::ptr event) {
    log(LogLevel::FATAL, event);
}

void Logger::addAppender(LogAppender::ptr appender) {
    if (!appender->getFormater()) {
        appender->setFormater(m_formatter);
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    for (auto i = m_appenders.begin();
        i != m_appenders.end(); ++i) {
        if (*i == appender) {
            m_appenders.erase(i);
            break;
        }
    }
}

std::string Logger::toYamlString() {
    YAML::Node node;
    node["name"] = m_name;
    if (m_level != LogLevel::UNKOWN) {
        node["level"] = LogLevel::ToString(m_level);
    }
    if (m_formatter) {
        node["formatter"] = m_formatter->getPattern();     
    }

    for (auto& i : m_appenders) {
        node["appenders"].push_back(YAML::Load(i->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

void LogAppender::setFormater(LogFormatter::ptr val) {
    m_formatter = val;
    if (m_formatter) {
        m_hasformatter = true;
    } else {
        m_hasformatter = false;
    }
}

std::string LoggerManager::toYamlstring() {
    YAML::Node node;
    for (auto& i : m_loggers) {
        node.push_back(YAML::Load(i.second->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

// 通过两种形式为logger设置formatter
void Logger::setFormatter(const std::string& formatter) {
    LogFormatter::ptr tmp(new LogFormatter(formatter));
    if (tmp->is_Error()) {
        std::cout << "formatter error" << std::endl;
    }
    //m_formatter = tmp;
    for (auto& i : m_appenders) {
        if (!i->m_hasformatter) {
            i->m_formatter = tmp;
        }
    }
    setFormatter(tmp);
}

void Logger::setFormatter(const LogFormatter::ptr formatter) {
    m_formatter = formatter;
}

FileLogAppender::FileLogAppender(const std::string& filename)
     : m_filename(filename) {
}

std::string FileLogAppender::toYamlString() {
    YAML::Node node;
    node["type"] = "FileLogAppender";
    node["file"] = m_filename;
    if (m_level != LogLevel::UNKOWN) {
        node["level"] = LogLevel::ToString(m_level);
    }

    if (m_hasformatter && m_formatter) {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) {
    if (level >= m_level) {
        m_filestream << m_formatter->format(logger, level, event);
    }
}

bool FileLogAppender::reopen(const std::string& filename) {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(filename);
    return !!m_filestream;
}

void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) {
    if (level >= m_level) {
        std::cout << m_formatter->format(logger, level, event);
    }
}
//头文件中声明了override 实现函数时不需要二次声明
std::string StdoutLogAppender::toYamlString() {
    YAML::Node node;
    node["type"] = "StdoutLogAppender";  

    if (m_level != LogLevel::UNKOWN) {
        node["level"] = LogLevel::ToString(m_level);
    }

    if (m_hasformatter && m_formatter) {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str(); 
}

LogFormatter::LogFormatter(const std::string& pattern) : m_pattern(pattern) {
    init();
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto &i : m_items) {
        i->format(logger, ss, level, event);
    }
    return ss.str();
}

//%XXX%   %% XXX{XXX} 可能解析这样的字符串
void LogFormatter::init() {
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for (size_t i = 0; i < m_pattern.size(); ++i) {
        //取出%之间的字符
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }
        //遇到连续的两个‘%’号，视为转义字符 存储一个%进字符串
        if ((i + 1) < m_pattern.size() && m_pattern[i + 1] == '%') {
            nstr.append(1, '%');
            continue;
        }

        //当前字符为‘%’ 且之后的字符串不是‘%’
        size_t n = i + 1;
        std::string fmt;
        std::string str;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        while (++n < m_pattern.size()) {
            //如果是空格直接退出循环，说明已经解析到了最后
            if (!isalpha(m_pattern[i]) && m_pattern[n] != '{' && m_pattern[n] != '}') {
                break;
            }
            //当前状态为0，即正在解析{之前的字符
            if (fmt_status == 0 && (n + 1) < m_pattern.size()) {
                if (m_pattern[n] == '{') {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    fmt_status = 1;
                    fmt_begin = n;
                    i = n;
                    continue;
                }
            }
            //当前状态为1，说明正在解析fmt，解析后更改状态
            if (fmt_status == 1 && (n + 1) < m_pattern.size()) {
                if (m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 2;
                }
            }
        }
        // 解析完一组之后 存入vec中
        if (fmt_status == 0) {
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, "", 0));
                nstr.clear();
            }
            str = m_pattern.substr(i + 1, n - i - 1);
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        } else if (fmt_status == 1) {
            //此时说明解析出错，并不是完整的一组 XXX{XXX}
            std::cout << "pattern parser error" << m_pattern << "-" << m_pattern.substr(i) << std::endl;
            vec.push_back(std::make_tuple("<<pattern error>>", fmt, 0));
            m_error = true;
        } else if (fmt_status == 2) {
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, "", 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        }
    }

    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
    //该map用来存储字符到格式子模块的映射,保存了全部存在的format
    static std::map<std::string, std::function<FormatterItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
        {#str, [] (const std::string& fmt) { return FormatterItem::ptr(new C(fmt)); }}

        XX(m, MessegeFormatItem),
        XX(p, LevelFormatItem),
        XX(r, ElapseFormatItem),
        XX(c, NameFormatItem),
        XX(t, ThreadIdFormatItem),
        XX(n, NextLineFormatItem),
        XX(d, DateTimeFormatItem),
        XX(f, FileNameFormatItem),
        XX(l, LineFormatItem),
        XX(T, TabFormatItem),
        //tab
        //...
#undef XX
    };

    for (auto& i : vec) {
        if (std::get<2>(i) == 0) {
            m_items.push_back(FormatterItem::ptr(new StirngFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if (it == s_format_items.end()) {
                m_items.push_back(FormatterItem::ptr(new StirngFormatItem("<<error_format %" + std::get<0>(i) + ">>"))); 
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
        //std::cout << std::get<0>(i) << "-" << std::get<1>(i) << "-" << std::get<2>(i) << std::endl;
    }
}

LoggerManager::LoggerManager() {
    m_root.reset(new Logger);

    m_root->addAppender(noobnet::StdoutLogAppender::ptr(new StdoutLogAppender));

    m_loggers[m_root->m_name] = m_root;

    init();
}

Logger::ptr LoggerManager::getLogger(const std::string& name) {
    auto i = m_loggers.find(name);

    if (i != m_loggers.end()) {
        return i->second;
    }

    Logger::ptr logger(new Logger(name));
    logger->m_root = m_root;
    m_loggers[name] = logger;
    return logger;
}

struct LogAppenderDefine {
    int type = 0; // 1 File, 2 Stdout
    LogLevel::level level = LogLevel::UNKOWN;
    std::string formatter;
    std::string file;

    bool operator== (const LogAppenderDefine& ohs) const {
        return type == ohs.type
            && level == ohs.level
            && formatter == ohs.formatter
            && file == ohs.file;
    }
};

struct LogDefine {
    std::string name;
    LogLevel::level level = LogLevel::UNKOWN;
    std::string formatter;
    std::vector<LogAppenderDefine> appenders;

    bool operator==(const LogDefine& ohs) const {
        return name == ohs.name
            && level == ohs.level
            && formatter == ohs.formatter
            && appenders == ohs.appenders;
    }

    bool operator<(const LogDefine& ohs) const {
        return name < ohs.name; //TODO
    }

    bool isValid() const {
        return !name.empty();
    }
};

//logdefine to string
template<>
class LexicalCast<LogDefine, std::string> {
public:
  std::string operator() (const LogDefine& i) {
    YAML::Node n;
    n["name"] = i.name;
    if (i.level != LogLevel::level::UNKOWN) {
        std::cout << "level works" << std::endl;
        n["level"] = LogLevel::ToString(i.level);
    }

    if (!i.formatter.empty()) {
        std::cout << "formatter works" << std::endl;
        n["formatter"] = i.formatter;
    }

    for (auto& a : i.appenders) {
        YAML::Node na;

        if (a.type == 1) {
            na["type"] = "FileLogAppender";
            na["file"] = a.file;
        } else if (a.type == 2) {
            na["type"] = "StdoutLogAppender";
        } 
        if (a.level != LogLevel::UNKOWN) {
            na["level"] = LogLevel::ToString(a.level);
        }
        if (!a.formatter.empty()) {
            na["formatter"] = a.formatter;
        }

        n["appenders"].push_back(na);
    }
    std::stringstream ss;
    ss << n;
    return ss.str();
  }
};

//string to logdefine
template<>
class LexicalCast<std::string, LogDefine> {
public:
  LogDefine operator() (const std::string& str) {
    YAML::Node n = YAML::Load(str);
    LogDefine ld;
    if (!n["name"].IsDefined()) {
        std::cout << "Logdefine's name is null" << std::endl;
        throw std::logic_error("log config name is null");
    }
    //std::cout << "name" << n["name"] << "level" << n["level"] << "formatter" << n["formatter"] << std::endl;
    ld.name = n["name"].as<std::string>();
    ld.level = LogLevel::FromString(n["level"].IsDefined() ? n["level"].as<std::string>() : "");
    if (n["formatter"].IsDefined()) {
        //std::cout << "formatter works" << std::endl;
        ld.formatter = n["formatter"].as<std::string>();
    }
    
    if (n["appenders"].IsDefined()) {
        //std::cout << "appenders parsering" << std::endl;
        for (size_t x = 0; x < n["appenders"].size(); ++x) {
            auto lap = n["appenders"][x];
            if (!lap["type"].IsDefined()) {
                std::cout << "log config error : type is null" << lap << std::endl;
                continue;
            }
            std::string type = lap["type"].as<std::string>();
            //std::cout << type << std::endl;
            LogAppenderDefine lad;
            if (type == "FileLogAppender") {
                lad.type = 1;
                if (!lap["file"].IsDefined()) {
                    std::cout << "log config error : file is null" << lap << std::endl;
                    continue;
                }
                lad.file = lap["file"].as<std::string>();
                if (lap["formatter"].IsDefined()) {
                    lad.formatter = lap["formatter"].as<std::string>();
                }
            } else if (type == "StdoutLogAppender") {
                lad.type = 2;
                if (lap["formatter"].IsDefined()) {
                    lad.formatter = lap["formatter"].as<std::string>();
                }
            } else {
                std::cout << "log config error : type is invalid" << lap << std::endl;
                continue;
            }
            ld.appenders.push_back(lad);
        }
    }
    return ld;
  }
};

noobnet::ConfigVar<std::set<LogDefine>>::ptr g_log_defines =
    noobnet::Config::lookUp(std::set<LogDefine>(), "logs", "log defines");

//TODO  fix
struct LogIniter {
    LogIniter() {
        g_log_defines->addListener([] (const std::set<LogDefine>& old_val,
            const std::set<LogDefine>& new_val) {
                SYS_LOG_INFO(SYS_LOG_ROOT()) << "on_logger_conf_changed";
                for (auto& i : new_val) {
                    auto it = old_val.find(i);
                    noobnet::Logger::ptr logger;
                    if (it == old_val.end()) {
                        logger = SYS_LOG_NAME(i.name);
                    } else {
                        if (!(i == *it)) {
                            logger = SYS_LOG_NAME(i.name);
                        } else {
                            continue;
                        }
                    } //logger fix finished
                    //logger's level and formatter
                    logger->setLevel(i.level);
                    if (!i.formatter.empty()) {
                        logger->setFormatter(i.formatter);
                    }
                    // appender's all 
                    for (auto& a : i.appenders) {
                        noobnet::LogAppender::ptr ap;
                        if (a.type == 1) {
                            ap.reset(new FileLogAppender(a.file));
                        } else if (a.type == 2) {
                            ap.reset(new StdoutLogAppender);
                        }
                        ap->setLevel(a.level);
                        if (!a.formatter.empty()) {
                            LogFormatter::ptr fm(new LogFormatter(a.formatter));
                            if (!fm->is_Error()) {
                                ap->setFormater(fm);
                            } else {
                                std::cout << "logdefines formatter invalid" << "-" <<
                                "log.name: " << i.name << "-" << "log.appender.format: "
                                << a.formatter << std::endl;
                            }
                        }
                        logger->addAppender(ap);
                    }
                }

                //delete logger
                for (auto& i : old_val) {
                    auto it = new_val.find(i);
                    if (it == new_val.end()) {
                        noobnet::Logger::ptr logger(SYS_LOG_NAME(i.name));
                        logger->setLevel((LogLevel::level)0);
                        logger->clearAppenders();
                    }
                }
            }        
        );
    }
};

//全局变量会在main函数执行之前进行初始化
static LogIniter __log_init_;

void LoggerManager::init() {

}
} // namespace noobnet
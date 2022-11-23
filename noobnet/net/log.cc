#include "log.h"

#include <map>
#include <functional>
#include <stdarg.h>


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

class MessegeFormatItem : public LogFormater::FormaterItem {
 public:
    MessegeFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getContent();
    }
};

class LevelFormatItem : public LogFormater::FormaterItem {
 public:
    LevelFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << LogLevel::ToString(level);
    }
};

class ElapseFormatItem : public LogFormater::FormaterItem {
 public:
    ElapseFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getElapse();
    }
};

class NameFormatItem : public LogFormater::FormaterItem {
 public:
    NameFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << logger->getName();
    }
};

class ThreadIdFormatItem : public LogFormater::FormaterItem {
 public:
    ThreadIdFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getThreadID();
    }
};

class FiberIdFormatItem : public LogFormater::FormaterItem {
 public:
    FiberIdFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getFiberID();
    }
};

class DateTimeFormatItem : public LogFormater::FormaterItem {
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

class LineFormatItem : public LogFormater::FormaterItem {
 public:
    LineFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getLineID();
    }
};

class NextLineFormatItem : public LogFormater::FormaterItem {
 public:
    NextLineFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class FileNameFormatItem : public LogFormater::FormaterItem {
 public:
    FileNameFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << event->getFile();
    }
};

class TabFormatItem : public LogFormater::FormaterItem {
 public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::shared_ptr<Logger> logger, std::ostream& os, LogLevel::level level, LogEvent::ptr event) override {
        os << "\t";
    }
};

class StirngFormatItem : public LogFormater::FormaterItem {
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
    m_formater.reset(new LogFormater("%d%T[%p]%T%c%T%m%T%n"));
}

void Logger::log(LogLevel::level level, LogEvent::ptr event) {
    if (level >= m_level) {
        auto self = shared_from_this();
        for (auto &i : m_appenders) {
            i->log(self , level, event);
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
        appender->setFormater(m_formater);
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

FileLogAppender::FileLogAppender(const std::string& filename)
     : m_filename(filename) {

}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) {
    if (level >= m_level) {
        m_filestream << m_formater->format(logger, level, event);
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
        std::cout << m_formater->format(logger, level, event);
    }
}

LogFormater::LogFormater(const std::string& pattern) : m_pattern(pattern) {
    init();
}

std::string LogFormater::format(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) {
    std::stringstream ss;
    for (auto &i : m_items) {
        i->format(logger, ss, level, event);
    }
    return ss.str();
}

//%XXX%   %% XXX{XXX} 可能解析这样的字符串
void LogFormater::init() {
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
    static std::map<std::string, std::function<FormaterItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
        {#str, [] (const std::string& fmt) { return FormaterItem::ptr(new C(fmt)); }}

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
            m_items.push_back(FormaterItem::ptr(new StirngFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if (it == s_format_items.end()) {
                m_items.push_back(FormaterItem::ptr(new StirngFormatItem("<<error_format %" + std::get<0>(i) + ">>"))); 
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

void LoggerManager::init() {

}
} // namespace noobnet
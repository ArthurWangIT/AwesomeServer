//
// Created by xing on 2023/4/23.
//

#ifndef __AWESOMESERVER_LOG_H__
#define __AWESOMESERVER_LOG_H__

#include <stdint.h>
#include <string>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>

namespace AwesomeServer {
    class LogLevel {
    public:
        enum Level {
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    };

    class LogEvent {
    public:
        typedef std::shared_ptr <LogEvent> ptr;

        LogEvent();

    private:
        const char *m_file = nullptr;
        int32_t m_line = 0;
        uint32_t m_threadId = 0;
        uint32_t m_fiberId = 0;
        uint64_t m_elapse = 0;
        uint64_t m_time = 0;
        std::string m_content;
    };

    class LogFormatter {
    public:
        typedef std::shared_ptr <LogFormatter> ptr;

        std::string format(LogEvent::ptr event);

    private:
    };

    class LogAppender {
    public:
        typedef std::shared_ptr <LogAppender> ptr;

        virtual ~LogAppender() {}

        virtual void log(LogLevel::Level level, LogEvent::ptr) = 0;

        void setFormatter(LogFormatter::ptr formatter) { m_formatter = formatter; }

        LogFormatter::ptr getFormatter() { return m_formatter; }

    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
    };

    class Logger {
    public:
        typedef std::shared_ptr <Logger> ptr;

        Logger(const std::string &name = "root");

        void log(LogLevel::Level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);

        void info(LogEvent::ptr event);

        void warn(LogEvent::ptr event);

        void error(LogEvent::ptr event);

        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);

        void delAppender(LogAppender::ptr appender);

        LogLevel::Level getLevel() const { return m_level; };

        void setLevel(LogLevel::Level level) { m_level = level; };

    private:
        std::string m_name;
        LogLevel::Level m_level;
        std::list <LogAppender::ptr> m_appenders;
    };

    class StdoutLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr <StdoutLogAppender> ptr;

        void log(LogLevel::Level level, LogEvent::ptr event) override;
    };

    class FileLogAppender : public LogAppender {
    public:
        typedef std::shared_ptr <FileLogAppender> ptr;

        FileLogAppender(const std::string &filename);

        void log(LogLevel::Level level, LogEvent::ptr event) override;

        bool reopen();

    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };
}

#endif //__AWESOMESERVER_LOG_H__

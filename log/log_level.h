/*************************************************************************
    > File Name: log_level.h
    > Author: hsz
    > Desc: This file is for log level.
    > Created Time: 2021年04月12日 星期一 22时16分04秒
 ************************************************************************/

#ifndef __LOG_LEVEL_H__
#define __LOG_LEVEL_H__

#include <string>

namespace Jarvis {
class LogLevel {
public:
    enum Level {
        DEBUG = 0,
        INFO  = 1,
        WARN  = 2,
        ERROR = 3,
        FATAL = 4
    };
    static std::string ToString(Level l)
    {
        std::string str;
        switch (l) {
        case DEBUG:
            str = "DEBUG";
            break;
        case INFO:
            str = "INFO";
            break;
        case WARN:
            str = "WARN";
            break;
        case ERROR:
            str = "ERROR";
            break;
        case FATAL:
            str = "FATAL";
            break;
        
        default:
            str = "";
            break;
        }
        return str;
    }

    static std::string ToFormatString(Level level)
    {
        std::string str;
        switch (level) {
        case DEBUG:
            str = "[D]";
            break;
        case INFO:
            str = "[I]";
            break;
        case WARN:
            str = "[W]";
            break;
        case ERROR:
            str = "[E]";
            break;
        case FATAL:
            str = "[F]";
            break;
        
        default:
            str = "[?]";
            break;
        }
        return str;
    }
};

} // namespace Jarvis
#endif // #ifndef __LOG_LEVEL_H__
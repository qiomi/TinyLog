#ifndef TINYLOG_UTILS_H_
#define TINYLOG_UTILS_H_

#include <ctime>
#include <string>

#include "tinylog/logger_types.h"
namespace tinylog::internal {

inline std::string GetCurrentTimestamp() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
}

inline const char* LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::kDebug:
            return "DEBUG";
        case LogLevel::kInfo:
            return "INFO";
        case LogLevel::kWarn:
            return "WARN";
        case LogLevel::kError:
            return "ERROR";
        case LogLevel::kFatal:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

inline LogLevel StringToLogLevel(const std::string& level_str) {
    if (level_str == "DEBUG") return LogLevel::kDebug;
    if (level_str == "INFO") return LogLevel::kInfo;
    if (level_str == "WARN") return LogLevel::kWarn;
    if (level_str == "ERROR") return LogLevel::kError;
    if (level_str == "FATAL") return LogLevel::kFatal;
    return LogLevel::kInfo;  // 默认级别
}

inline LogSink StringToLogSink(const std::string& sink_str) {
    if (sink_str == "CONSOLE") return LogSink::kConsole;
    if (sink_str == "FILE") return LogSink::kFile;
    if (sink_str == "BOTH") return LogSink::kBoth;
    return LogSink::kConsole;
}

inline std::string LogSinkToString(LogSink sink) {
    switch (sink) {
        case LogSink::kConsole:
            return "CONSOLE";
        case LogSink::kFile:
            return "FILE";
        case LogSink::kBoth:
            return "BOTH";
        default:
            return "UNKNOWN";
    }
}

}  // namespace tinylog::internal

#endif  // TINYLOG_UTILS_H_
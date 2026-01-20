#include "tinylog/internal/log_utils.h"
#include <ctime>
#include <fstream>
#include <string>
#include <algorithm>
#include <sys/stat.h>

namespace tinylog::internal {

std::string GetCurrentTimestamp() {
    time_t now = time(nullptr);
    struct tm local_time;
    localtime_r(&now, &local_time);
    
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_time);
    return std::string(buffer);
}

const char* LogLevelToString(LogLevel level) {
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

LogLevel StringToLogLevel(const std::string& level_str) {
    std::string lower_str = level_str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    
    if (lower_str == "debug") {
        return LogLevel::kDebug;
    } else if (lower_str == "info") {
        return LogLevel::kInfo;
    } else if (lower_str == "warn") {
        return LogLevel::kWarn;
    } else if (lower_str == "error") {
        return LogLevel::kError;
    } else if (lower_str == "fatal") {
        return LogLevel::kFatal;
    } else {
        return LogLevel::kInfo;  // 默认返回INFO级别
    }
}

std::string LogSinkToString(LogSink sink) {
    switch (sink) {
        case LogSink::kConsole:
            return "console";
        case LogSink::kFile:
            return "file";
        case LogSink::kBoth:
            return "both";
        default:
            return "unknown";
    }
}

LogSink StringToLogSink(const std::string& sink_str) {
    std::string lower_str = sink_str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    
    if (lower_str == "console") {
        return LogSink::kConsole;
    } else if (lower_str == "file") {
        return LogSink::kFile;
    } else if (lower_str == "both") {
        return LogSink::kBoth;
    } else {
        return LogSink::kConsole;  // 默认返回控制台输出
    }
}

void Trim(std::string& str) {
    // 去除开头的空格和制表符
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) {
        str.clear();
        return;
    }
    
    // 去除结尾的空格和制表符
    size_t end = str.find_last_not_of(" \t");
    str = str.substr(start, end - start + 1);
}

time_t GetFileLastModifiedTime(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::in);
    if (!file) {
        return 0;
    }
    
    time_t last_modified = 0;
    struct stat file_stat;
    if (stat(file_path.c_str(), &file_stat) == 0) {
        last_modified = file_stat.st_mtime;
    }
    
    file.close();
    return last_modified;
}

}  // namespace tinylog::internal

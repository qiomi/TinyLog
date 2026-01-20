#ifndef TINYLOG_INTERNAL_LOG_UTILS_H_
#define TINYLOG_INTERNAL_LOG_UTILS_H_

#include <string>

#include "tinylog/log_level.h"

namespace tinylog::internal {

// 获取当前时间戳，格式为YYYY-MM-DD HH:MM:SS
std::string GetCurrentTimestamp();

// 将日志级别转换为字符串
const char* LogLevelToString(LogLevel level);

// 将字符串转换为日志级别
LogLevel StringToLogLevel(const std::string& level_str);

// 将日志输出目标转换为字符串
std::string LogSinkToString(LogSink sink);

// 将字符串转换为日志输出目标
LogSink StringToLogSink(const std::string& sink_str);

// 去除字符串首尾的空格和制表符
void Trim(std::string& str);

// 获取文件的最后修改时间
time_t GetFileLastModifiedTime(const std::string& file_path);

}  // namespace tinylog::internal

#endif  // TINYLOG_INTERNAL_LOG_UTILS_H_

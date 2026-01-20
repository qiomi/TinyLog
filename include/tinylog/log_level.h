#ifndef TINYLOG_LOG_LEVEL_H_
#define TINYLOG_LOG_LEVEL_H_

namespace tinylog {

// 日志级别枚举
enum class LogLevel { kDebug, kInfo, kWarn, kError, kFatal };

// 日志输出目标枚举
enum class LogSink { kConsole, kFile, kBoth };

}  // namespace tinylog

#endif  // TINYLOG_LOG_LEVEL_H_

#ifndef TINYLOG_LOGGER_H_
#define TINYLOG_LOGGER_H_

#include <memory>
#include <string>

#include "tinylog/logger_types.h"

namespace tinylog {
namespace internal {
class LogSinkInterface;
};  // namespace internal
class Logger {
public:
    explicit Logger(const LogConfig& config = LogConfig());
    explicit Logger(const std::string& config_file_path);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    Logger(Logger&&) noexcept = default;
    Logger& operator=(Logger&&) noexcept = default;

    ~Logger() = default;

    void Log(LogLevel level, const std::string& message);

    void LogDebug(const std::string& message) { Log(LogLevel::kDebug, message); }
    void LogInfo(const std::string& message) { Log(LogLevel::kInfo, message); }
    void LogWarn(const std::string& message) { Log(LogLevel::kWarn, message); }
    void LogError(const std::string& message) { Log(LogLevel::kError, message); }
    void LogFatal(const std::string& message) { Log(LogLevel::kFatal, message); }

    void SetLogLevel(LogLevel level);
    LogLevel GetLogLevel() const;

    void SetLogSink(LogSink sink);
    LogSink GetLogSink() const;

    void Flush();

private:
    LogConfig config_;

    std::unique_ptr<internal::LogSinkInterface> console_sink_;
    std::unique_ptr<internal::LogSinkInterface> file_sink_;

    std::string FormatMessage(LogLevel level, const std::string& message);
    void InitSinks();
    void LoadConfigFromFile(const std::string& config_file_path);
};

}  // namespace tinylog

#endif  // TINYLOG_LOGGER_H_
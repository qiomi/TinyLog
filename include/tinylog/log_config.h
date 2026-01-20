#ifndef TINYLOG_LOG_CONFIG_H_
#define TINYLOG_LOG_CONFIG_H_

#include <string>
#include "log_level.h"

namespace tinylog {

// 日志配置类，用于配置日志的各种参数
class LogConfig {
public:
    LogConfig();
    LogConfig(LogLevel level, LogSink sink, const std::string& file_path, int32_t max_file_count, size_t max_file_size,
              bool async_mode);
    LogConfig(const LogConfig&) = default;
    LogConfig& operator=(const LogConfig&) = default;
    LogConfig(LogConfig&&) noexcept = default;
    LogConfig& operator=(LogConfig&&) noexcept = default;
    ~LogConfig() = default;

    // 设置日志级别
    void SetLogLevel(LogLevel level);
    // 获取日志级别
    LogLevel GetLogLevel() const noexcept;

    // 设置日志输出目标
    void SetLogSink(LogSink sink);
    // 获取日志输出目标
    LogSink GetLogSink() const noexcept;

    // 设置日志文件路径
    void SetFilePath(const std::string& path);
    // 获取日志文件路径
    const std::string& GetFilePath() const noexcept;

    // 设置最大文件数量
    void SetMaxFileCount(int32_t count);
    // 获取最大文件数量
    int32_t GetMaxFileCount() const noexcept;

    // 设置单个文件最大大小
    void SetMaxFileSize(size_t size);
    // 获取单个文件最大大小
    size_t GetMaxFileSize() const noexcept;

    // 设置异步模式
    void SetAsyncMode(bool async);
    // 获取是否为异步模式
    bool IsAsyncMode() const noexcept;

    // 重置为默认配置
    void ResetToDefault();

    // 验证配置是否有效
    bool Validate() const;

private:
    // 检查文件数量是否有效
    bool IsValidFileCount(int32_t count) const noexcept;
    // 检查文件大小是否有效
    bool IsValidFileSize(size_t size) const noexcept;

    LogLevel level_;
    LogSink sink_;
    std::string file_path_;
    int32_t max_file_count_;
    size_t max_file_size_;
    bool async_mode_;

    // 默认配置值
    static constexpr LogLevel kDefaultLogLevel = LogLevel::kInfo;
    static constexpr LogSink kDefaultLogSink = LogSink::kConsole;
    static constexpr int32_t kDefaultMaxFileCount = 5;
    static constexpr size_t kDefaultMaxFileSize = 1024 * 1024;  // 1MB
    static constexpr bool kDefaultAsyncMode = false;
};

}  // namespace tinylog

#endif  // TINYLOG_LOG_CONFIG_H_

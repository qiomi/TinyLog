#ifndef LOGGER_TYPES_H
#define LOGGER_TYPES_H
#include <string>

namespace tinylog {

enum class LogLevel { kDebug, kInfo, kWarn, kError, kFatal };

enum class LogSink { kConsole, kFile, kBoth };

class LogConfig {
public:
    LogConfig();
    LogConfig(LogLevel level, LogSink sink, const std::string& file_path, int32_t max_file_count, size_t max_file_size,
              bool async_mode);
    LogConfig(LogConfig&&) noexcept = default;
    LogConfig& operator=(LogConfig&&) noexcept = default;
    ~LogConfig() = default;

    void SetLogLevel(LogLevel level);
    LogLevel GetLogLevel() const noexcept;

    void SetLogSink(LogSink sink);
    LogSink GetLogSink() const noexcept;

    void SetFilePath(const std::string& path);
    const std::string& GetFilePath() const noexcept;

    void SetMaxFileCount(int32_t count);
    int32_t GetMaxFileCount() const noexcept;

    void SetMaxFileSize(size_t size);
    size_t GetMaxFileSize() const noexcept;

    void SetAsyncMode(bool async);
    bool IsAsyncMode() const noexcept;

    void ResetToDefault();

    bool Validate() const;

private:
    LogLevel level_;
    LogSink sink_;
    std::string file_path_;
    int32_t max_file_count_;
    size_t max_file_size_;
    bool async_mode_;

    static constexpr LogLevel kDefaultLogLevel = LogLevel::kInfo;
    static constexpr LogSink kDefaultLogSink = LogSink::kConsole;
    static constexpr int32_t kDefaultMaxFileCount = 5;
    static constexpr size_t kDefaultMaxFileSize = 1024 * 1024;  // 1MB
    static constexpr bool kDefaultAsyncMode = false;

    bool IsValidFileCount(int32_t count) const noexcept;
    bool IsValidFileSize(size_t size) const noexcept;
};
}  // namespace tinylog
#endif  // LOGGER_TYPES_H
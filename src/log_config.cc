#include "tinylog/log_config.h"

#include <string>

namespace tinylog {

LogConfig::LogConfig()
    : level_(kDefaultLogLevel),
      sink_(kDefaultLogSink),
      file_path_(""),
      max_file_count_(kDefaultMaxFileCount),
      max_file_size_(kDefaultMaxFileSize),
      async_mode_(kDefaultAsyncMode) {}

LogConfig::LogConfig(LogLevel level, LogSink sink, const std::string& file_path, int32_t max_file_count,
                     size_t max_file_size, bool async_mode)
    : level_(level),
      sink_(sink),
      file_path_(file_path),
      max_file_count_(max_file_count),
      max_file_size_(max_file_size),
      async_mode_(async_mode) {
    Validate();
}

void LogConfig::SetLogLevel(LogLevel level) { level_ = level; }

LogLevel LogConfig::GetLogLevel() const noexcept { return level_; }

void LogConfig::SetLogSink(LogSink sink) { sink_ = sink; }

LogSink LogConfig::GetLogSink() const noexcept { return sink_; }

void LogConfig::SetFilePath(const std::string& path) { file_path_ = path; }

const std::string& LogConfig::GetFilePath() const noexcept { return file_path_; }

void LogConfig::SetMaxFileCount(int32_t count) {
    if (IsValidFileCount(count)) {
        max_file_count_ = count;
    }
}

int32_t LogConfig::GetMaxFileCount() const noexcept { return max_file_count_; }

void LogConfig::SetMaxFileSize(size_t size) {
    if (IsValidFileSize(size)) {
        max_file_size_ = size;
    }
}

size_t LogConfig::GetMaxFileSize() const noexcept { return max_file_size_; }

void LogConfig::SetAsyncMode(bool async) { async_mode_ = async; }

bool LogConfig::IsAsyncMode() const noexcept { return async_mode_; }

void LogConfig::ResetToDefault() {
    level_ = kDefaultLogLevel;
    sink_ = kDefaultLogSink;
    file_path_ = "";
    max_file_count_ = kDefaultMaxFileCount;
    max_file_size_ = kDefaultMaxFileSize;
    async_mode_ = kDefaultAsyncMode;
}

bool LogConfig::Validate() const {
    return IsValidFileCount(max_file_count_) && IsValidFileSize(max_file_size_) && !file_path_.empty();
}

bool LogConfig::IsValidFileCount(int32_t count) const noexcept { return count >= 1 && count <= 100; }

bool LogConfig::IsValidFileSize(size_t size) const noexcept {
    return size >= 1024 && size <= 1024 * 1024 * 1024;  // 1KB - 1GB
}

}  // namespace tinylog

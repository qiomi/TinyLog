#include "tinylog/internal/sink_interface.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>

#include "tinylog/internal/log_utils.h"

namespace tinylog::internal {

void SinkInterface::log(const LogEvent& event) {
    std::string formatted_message = format(event);
    write(formatted_message);
}

std::string SinkInterface::format(const LogEvent& event) {
    std::string timestamp = GetCurrentTimestamp();
    const char* level_str = LogLevelToString(event.level);

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "[%s] [%s] %s:%s:%d - %s\n", timestamp.c_str(), level_str, event.filename,
             event.function, event.line, event.message.c_str());

    return std::string(buffer);
}

// ConsoleSink implementation
void ConsoleSink::write(const std::string& message) {
    fputs(message.c_str(), stdout);
    fflush(stdout);
}

// FileSink implementation
FileSink::FileSink(const std::string& file_path, int32_t max_file_count, size_t max_file_size)
    : file_path_(file_path), max_file_count_(max_file_count), max_file_size_(max_file_size) {
    // 打开日志文件
    log_file_.open(file_path_, std::ios::out | std::ios::app);
    if (!log_file_.is_open()) {
        fprintf(stderr, "Failed to open log file: %s\n", file_path_.c_str());
    }
}

FileSink::~FileSink() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void FileSink::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(file_mutex_);

    if (!log_file_.is_open()) {
        log_file_.open(file_path_, std::ios::out | std::ios::app);
        if (!log_file_.is_open()) {
            return;
        }
    }

    // 检查是否需要滚动文件
    if (shouldRotateFile()) {
        rotateFile();
    }

    // 写入日志
    log_file_ << message;

    // 检查写入是否成功
    if (log_file_.fail()) {
        fprintf(stderr, "Failed to write to log file: %s\n", file_path_.c_str());
        log_file_.clear();
    }
}

void FileSink::flush() {
    std::lock_guard<std::mutex> lock(file_mutex_);
    if (log_file_.is_open()) {
        log_file_.flush();
    }
}

void FileSink::rotateFile() {
    std::lock_guard<std::mutex> lock(file_mutex_);

    // 关闭当前日志文件
    if (log_file_.is_open()) {
        log_file_.close();
    }

    // 生成备份文件名并移动
    for (int i = max_file_count_ - 1; i > 0; --i) {
        std::string old_file = file_path_ + "." + std::to_string(i);
        std::string new_file = file_path_ + "." + std::to_string(i + 1);

        if (std::filesystem::exists(old_file)) {
            std::filesystem::rename(old_file, new_file);
        }
    }

    // 重命名当前日志文件为 .1
    std::string backup_file = file_path_ + ".1";
    if (std::filesystem::exists(file_path_)) {
        std::filesystem::rename(file_path_, backup_file);
    }

    // 重新打开新的日志文件
    log_file_.open(file_path_, std::ios::out | std::ios::trunc);
    if (!log_file_.is_open()) {
        fprintf(stderr, "Failed to create new log file: %s\n", file_path_.c_str());
    }
}

bool FileSink::shouldRotateFile() const {
    if (!log_file_.is_open()) {
        return false;
    }

    log_file_.seekp(0, std::ios::end);
    std::streampos pos = log_file_.tellp();
    return static_cast<size_t>(pos) >= max_file_size_;
}

}  // namespace tinylog::internal

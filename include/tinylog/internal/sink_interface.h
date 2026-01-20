#ifndef TINYLOG_INTERNAL_SINK_INTERFACE_H_
#define TINYLOG_INTERNAL_SINK_INTERFACE_H_

#include <ctime>
#include <fstream>
#include <mutex>
#include <string>

#include "tinylog/log_level.h"

namespace tinylog::internal {

struct LogEvent {
    std::string message;   // 日志内容
    LogLevel level;        // 日志级别
    time_t timestamp;      // 时间戳（格式：YYYY-MM-DD HH:MM:SS）
    const char* filename;  // 触发日志的文件名
    const char* function;  // 触发日志的函数名
    int line = 0;          // 触发日志的行号
};

class SinkInterface {
public:
    SinkInterface() = default;
    virtual ~SinkInterface() = default;

    SinkInterface(const SinkInterface&) = delete;
    SinkInterface& operator=(const SinkInterface&) = delete;
    SinkInterface(SinkInterface&&) = default;
    SinkInterface& operator=(SinkInterface&&) = default;

    // 处理日志事件，会调用format和write方法
    void log(const LogEvent& event);
    // 刷新日志缓存
    virtual void flush() {}

protected:
    // 写入格式化后的日志消息，子类必须实现
    virtual void write(const std::string& message) = 0;

private:
    // 格式化日志事件
    std::string format(const LogEvent& event);
};

class ConsoleSink : public SinkInterface {
protected:
    void write(const std::string& message) override;
};

class FileSink : public SinkInterface {
public:
    explicit FileSink(const std::string& file_path, int32_t max_file_count = 5, size_t max_file_size = 1024 * 1024);
    ~FileSink() override;

    FileSink(const FileSink&) = delete;
    FileSink& operator=(const FileSink&) = delete;
    FileSink(FileSink&&) noexcept = default;
    FileSink& operator=(FileSink&&) noexcept = default;

protected:
    void write(const std::string& message) override;
    void flush() override;

private:
    void rotateFile();
    bool shouldRotateFile() const;

    std::string file_path_;
    int32_t max_file_count_;
    size_t max_file_size_;
    mutable std::ofstream log_file_;
    mutable std::mutex file_mutex_;
};

}  // namespace tinylog::internal

#endif  // TINYLOG_INTERNAL_SINK_INTERFACE_H_

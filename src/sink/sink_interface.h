#ifndef TINYLOG_SINK_INTERFACE_H_
#define TINYLOG_SINK_INTERFACE_H_

#include <ctime>
#include <memory>
#include <string>

#include "tinylog/logger_types.h"

// 谷歌规范：命名空间小写，与项目名/模块匹配
namespace tinylog::internal {

struct LogEvent {
    std::string message;   // 日志内容
    LogLevel level;        // 日志级别
    time_t timestamp;      // 时间戳（格式：YYYY-MM-DD HH:MM:SS）
    std::string filename;  // 触发日志的文件名
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

    virtual void log(const LogEvent& event);
    virtual void flush() {}
    static std::shared_ptr<SinkInterface> CreateSink(LogSink type, const std::string& filepath = "");

protected:
    virtual void log(const std::string& message) = 0;

private:
    virtual std::string format(const LogEvent& event);
};
};  // namespace tinylog::internal

#endif  // TINYLOG_SINK_INTERFACE_H_

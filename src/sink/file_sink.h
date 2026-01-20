#ifndef TINYLOG_FILE_SINK_H_
#define TINYLOG_FILE_SINK_H_

#include <cstdio>
#include <mutex>
#include <string>

#include "sink_interface.h"

namespace tinylog::internal {

class FileSink : public SinkInterface {
public:
    explicit FileSink(const std::string& filepath);
    ~FileSink() override;

    void log(const std::string& message) override;
    void flush() override;

private:
    std::string filepath_;      // 日志文件路径（成员变量加下划线，谷歌规范）
    FILE* file_ptr_ = nullptr;  // 文件指针（优先FILE*，日志场景性能更优）
    std::mutex mutex_;          // 互斥锁：保证多线程写文件安全
};
}  // namespace tinylog::internal

#endif  // TINYLOG_FILE_SINK_H_
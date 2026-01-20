#ifndef TINYLOG_CONSOLE_SINK_H_
#define TINYLOG_CONSOLE_SINK_H_

#include "sink_interface.h"

namespace tinylog::internal {

class ConsoleSink : public SinkInterface {
public:
    ConsoleSink() = default;
    ~ConsoleSink() override = default;
    void log(const std::string& message) override;
    void flush() override;
};

}  // namespace tinylog::internal

#endif  // TINYLOG_CONSOLE_SINK_H_
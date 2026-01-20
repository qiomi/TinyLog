#include "console_sink.h"

#include <iostream>
namespace tinylog::internal {

void ConsoleSink::log(const std::string& message) { std::cout << message; }

void ConsoleSink::flush() { std::cout.flush(); }

}  // namespace tinylog::internal
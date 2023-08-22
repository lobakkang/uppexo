#include <cstdarg>
#include <cstdio>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utils/log.hpp>

void uppexo::Log::logInfo(const char *format, ...) {
  if (level >= LOG_LEVEL_INFO) {
    va_list args;
    va_start(args, format);
    va_list argscopy;
    va_copy(argscopy, args);
    size_t size = std::vsnprintf(nullptr, 0, format, args) + 1;
    std::unique_ptr<char[]> buffer(new char[size]);
    std::vsnprintf(buffer.get(), size, format, argscopy);
    va_end(args);
    std::string string_buffer =
        std::string("[\033[32mNORMAL \033[0m]") +
        std::string(buffer.get(), buffer.get() + size - 1);
    std::cout << string_buffer << std::flush;
  }
}

void uppexo::Log::logVerbose(const char *format, ...) {
  if (level >= LOG_LEVEL_VERBOSE) {
    va_list args;
    va_start(args, format);
    va_list argscopy;
    va_copy(argscopy, args);
    size_t size = std::vsnprintf(nullptr, 0, format, args) + 1;
    std::unique_ptr<char[]> buffer(new char[size]);
    std::vsnprintf(buffer.get(), size, format, argscopy);
    va_end(args);
    std::string string_buffer =
        std::string("[\033[1;34mVERBOSE\033[0m]") +
        std::string(buffer.get(), buffer.get() + size - 1);
    std::cout << string_buffer << std::flush;
  }
}

void uppexo::Log::logWarning(const char *format, ...) {
  if (level >= LOG_LEVEL_WARNING) {
    va_list args;
    va_start(args, format);
    va_list argscopy;
    va_copy(argscopy, args);
    size_t size = std::vsnprintf(nullptr, 0, format, args) + 1;
    std::unique_ptr<char[]> buffer(new char[size]);
    std::vsnprintf(buffer.get(), size, format, argscopy);
    va_end(args);
    std::string string_buffer =
        std::string("[\033[1;33mWARNING\033[0m]") +
        std::string(buffer.get(), buffer.get() + size - 1);
    std::cout << string_buffer << std::flush;
  }
}

void uppexo::Log::logError(const char *format, ...) {
  va_list args;
  va_start(args, format);
  va_list argscopy;
  va_copy(argscopy, args);
  size_t size = std::vsnprintf(nullptr, 0, format, args) + 1;
  std::unique_ptr<char[]> buffer(new char[size]);
  std::vsnprintf(buffer.get(), size, format, argscopy);
  va_end(args);
  std::string string_buffer =
      std::string("[\033[1;31mERROR  \033[0m]") +
      std::string(buffer.get(), buffer.get() + size - 1);
  std::cout << string_buffer << std::flush;
  throw std::runtime_error("Error fault triggered!");
}

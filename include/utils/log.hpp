#ifndef LOG_H
#define LOG_H

namespace uppexo {

enum LogLevel {
  LOG_LEVEL_INFO = 1,
  LOG_LEVEL_WARNING = 2,
  LOG_LEVEL_VERBOSE = 3,
};

class Log {
public:
  static Log &GetInstance() {
    static Log instance;
    return instance;
  }

  void logInfo(const char *format, ...);
  void logVerbose(const char *format, ...);
  void logWarning(const char *format, ...);
  void logError(const char *format, ...);

  void setLevel(LogLevel level) { this->level = level; }
  LogLevel getLevel() { return level; }

private:
  LogLevel level = LOG_LEVEL_VERBOSE;

  Log() = default;
  ~Log() = default;
  Log(const Log &) = delete;
  Log &operator=(const Log &) = delete;
};
}; // namespace uppexo

#endif /* end of include guard: LOG_H */

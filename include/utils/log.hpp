#ifndef LOG_H
#define LOG_H

namespace uppexo {
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

private:
  Log() = default;
  ~Log() = default;
  Log(const Log &) = delete;
  Log &operator=(const Log &) = delete;
};
}; // namespace uppexo

#endif /* end of include guard: LOG_H */

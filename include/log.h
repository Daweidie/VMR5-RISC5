#ifndef LOG_H
#define LOG_H

#ifdef DEBUG
#define DEBUG_LOG(fmt, ...) log_print("DEBUG", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#else
#define DEBUG_LOG(fmt, ...) do {} while(0)
#endif

#define INFO_LOG(fmt, ...)  log_print("INFO ", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define WARN_LOG(fmt, ...)  log_print("WARN ", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) log_print("ERROR", __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

void log_print(const char *level, const char *file, int line, const char *func, const char *fmt, ...);

#endif
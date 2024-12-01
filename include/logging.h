#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL DEBUG
#endif

#define LOG_FORMAT "%s %s %s:%s:%d "
#define MAX_FORMAT_SIZE 2048 
#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S" 
#define TIMESTAMP_SIZE 26

// TODO: Add millisecond timestamp precision
#define LOG_PRINT(lvl, fmt, ...) do { \
        time_t now = time(NULL); \
        struct tm* tm_info = localtime(&now); \
        char timestamp[ TIMESTAMP_SIZE ]; \
        strftime(timestamp, TIMESTAMP_SIZE, TIMESTAMP_FORMAT, tm_info); \
        fprintf(stdout, LOG_FORMAT fmt "\n", \
                timestamp, \
                getLevelText(lvl), \
                __FILE__, \
                __func__, \
                __LINE__, \
                ##__VA_ARGS__); \
} while(0)

#define log_debug(fmt, ...) LOG_PRINT(DEBUG, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) LOG_PRINT(INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) LOG_PRINT(WARN, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) LOG_PRINT(ERROR, fmt, ##__VA_ARGS__)

extern int log_level;
extern int color_enabled;

enum LOG_LEVEL {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
};

char* getLevelText(enum LOG_LEVEL level);

#endif /* __LOGGING_H__ */

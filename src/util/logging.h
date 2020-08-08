#ifndef LOGGING_H
#define LOGGING_H

#include <stdarg.h>
#include <stdio.h>

#define NONE 5
#define CRITICAL 4
#define ERROR 3
#define WARNING 2
#define INFO 1
#define DEBUG 0

static int log_level = INFO;

struct Logging {
    void (*log)(int level, char* message);
    void (*logf)(int level, char* message, ...);
};

static void logging_logf(int level, char* message, ...) {
    if(level >= log_level) {
        va_list args;
        va_start(args, message);
        vprintf(message, args);
        va_end(args);
    }
    fflush(NULL);
}

static void logging_log(int level, char* message) {
    if(level >= log_level) {
        printf(message);
    }
    fflush(NULL);
}

static struct Logging logging = { &logging_log, &logging_logf };

#endif
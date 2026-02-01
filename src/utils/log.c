#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "../include/log.h"

void log_print(const char *level, const char *file, int line, const char *func, const char *fmt, ...) {
    // Extract just the filename from the full path
    const char *filename = strrchr(file, '/');
    if (!filename) {
        filename = strrchr(file, '\\');
    }
    if (!filename) {
        filename = file;
    } else {
        filename++; // Skip the slash
    }

    printf("[%s] %s:%d (%s) - ", level, filename, line, func);
    
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    
    printf("\n");
    fflush(stdout);
}
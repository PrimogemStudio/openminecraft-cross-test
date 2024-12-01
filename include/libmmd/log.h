#ifndef _LOG_H
#define _LOG_H

#include "stdlib.h"
#include "stdio.h"

typedef void (*mmd_log_handler)(int type, char* file, int line, char* msg);
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3
#define LOG_FATAL 4

int mmd_log_set_handler(mmd_log_handler handler);
void mmd_log(int type, char* file, int line, char* msg);

#define LIBMMD_LOG(type, msg) mmd_log(type, __FILE__, __LINE__, msg)

#endif

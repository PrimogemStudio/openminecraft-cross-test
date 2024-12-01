#include "libmmd/log.h"
#include "libmmd/return_codes.h"
#include "stdio.h"

void immd_log_impl(int type, char* file, int line, char* msg)
{
    printf("%d, %s, %d, %s\n", type, file, line, msg);
}
mmd_log_handler _handler = immd_log_impl;

int mmd_log_set_handler(mmd_log_handler handler)
{
    if (!handler) return MMD_NULL_PTR;
    _handler = handler;
    return MMD_NO_ERROR; 
}
void mmd_log(int type, char* file, int line, char* msg)
{
    _handler(type, file, line, msg);
}

#ifndef _MEMORY_H
#define _MEMORY_H

#include "stddef.h"
#include "stdlib.h"

typedef void* (*mmd_memory_allocator) (size_t bytes);

int mmd_memory_set_allocator(mmd_memory_allocator allocator);
void* mmd_memory_allocate(size_t bytes);

#endif

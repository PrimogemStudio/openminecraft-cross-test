#ifndef _MEMORY_H
#define _MEMORY_H

#include "stddef.h"
#include "stdlib.h"

typedef void* (*mmd_memory_allocator) (size_t bytes);
typedef void (*mmd_memory_deallocator) (void* buf);
typedef void (*mmd_memory_duplicator) (void* dest, const void* src, size_t bytes); 

extern "C" {

int mmd_memory_set_allocator(mmd_memory_allocator allocator);
void* mmd_memory_allocate(size_t bytes);
int mmd_memory_set_deallocator(mmd_memory_deallocator deallocator);
int mmd_memory_deallocate(void* buf);
int mmd_memory_set_duplicator(mmd_memory_duplicator duplicator);
int mmd_memory_duplicate(void* dest, const void* src, size_t bytes);

#define mmd_memory_allocate_struct(T) (T*) mmd_memory_allocate(sizeof(T))
#define mmd_memory_allocate_array(T, l) (T*) mmd_memory_allocate(sizeof(T) * l)

}

#endif

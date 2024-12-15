#include "libmmd/memory.h"
#include "libmmd/return_codes.h"
#include "stdio.h"
#include "stdint.h"
#include <cstring>

void* mmdi_memory_allocate(size_t bytes)
{
    void* buf = malloc(bytes);
    memset(buf, 0, bytes);
    return buf;
}

mmd_memory_allocator _allocator = mmdi_memory_allocate;
mmd_memory_deallocator _deallocator = free;
mmd_memory_duplicator _duplicator = (mmd_memory_duplicator) memcpy;

int mmd_memory_set_allocator(mmd_memory_allocator allocator)
{
    if (!allocator) return MMD_NULL_PTR;
    _allocator = allocator;
    return MMD_NO_ERROR;
}

void* mmd_memory_allocate(size_t bytes)
{
    return _allocator(bytes);
}

int mmd_memory_set_deallocator(mmd_memory_deallocator deallocator)
{
    if (!deallocator) return MMD_NULL_PTR;
    _deallocator = deallocator;
    return MMD_NO_ERROR;
}

int mmd_memory_deallocate(void* buf)
{
    if (!buf) return MMD_NULL_PTR;
    _deallocator(buf);
    return MMD_NO_ERROR;
}

int mmd_memory_set_duplicator(mmd_memory_duplicator duplicator)
{
    if (!duplicator) return MMD_NULL_PTR;
    _duplicator = duplicator;
    return MMD_NO_ERROR;
}

int mmd_memory_duplicate(void* dest, const void* src, size_t bytes)
{
    if (!dest || !src || bytes <= 0) return MMD_NULL_PTR;
    _duplicator(dest, src, bytes);
    return MMD_NO_ERROR;
}

#include "libmmd/memory.h"
#include "libmmd/return_codes.h"
#include "stdio.h"
#include "stdint.h"

void* mmdi_memory_allocate(size_t bytes)
{
    void* buf = malloc(bytes);
    printf("mmdi_memory_allocate 0x%lx\n", (uint64_t) buf);
    return buf;
}

void mmdi_memory_deallocate(void* buf)
{
    printf("mmdi_memory_deallocate 0x%lx\n", (uint64_t) buf);
    free(buf);
}

mmd_memory_allocator _allocator = mmdi_memory_allocate;
mmd_memory_deallocator _deallocator = mmdi_memory_deallocate;

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

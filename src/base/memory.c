#include "libmmd/memory.h"
#include "libmmd/return_codes.h"

mmd_memory_allocator _allocator = malloc;
int mmd_memory_set_allocator(mmd_memory_allocator allocator)
{
    if (!allocator) return MMD_NULL_PTR;
    _allocator = allocator;
}
void* mmd_memory_allocate(size_t bytes)
{
    return _allocator(bytes);
}

#include "libmmd/memory.h"
#include "libmmd/return_codes.h"
#include "libmmd/log.h"

mmd_memory_allocator _allocator = malloc;
int mmd_memory_set_allocator(mmd_memory_allocator allocator)
{
    if (!allocator) return MMD_NULL_PTR;
    _allocator = allocator;
    return MMD_NO_ERROR;
}
void* mmd_memory_allocate(size_t bytes)
{
    LIBMMD_LOG(LOG_DEBUG, "allocating!");
    return _allocator(bytes);
}

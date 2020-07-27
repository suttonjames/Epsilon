#include "memory.h"

void alloc_arena(MemoryArena *arena, usize size, u64 *base)
{
    arena->size = size;
    arena->base = base;
    arena->used = 0;
}

void *push_memory(MemoryArena *arena, usize size)
{
    assert((arena->used + size) <= arena->size);
    void *result = arena->base + arena->used;
    arena->used += size;
    return result;
}

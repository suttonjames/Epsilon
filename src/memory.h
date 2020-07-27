#ifndef MEMORY_H
#define MEMORY_H

typedef struct MemoryArena {
    usize size;
    u64 *base;
    usize used;
} MemoryArena;

#define push_struct(arena, type) (type *)push_memory(arena, sizeof(type))
#define push_array(arena, count, type) (type *)push_memory(arena, count * sizeof(type))

void alloc_arena(MemoryArena *arena, usize size, u64 *base);
void *push_memory(MemoryArena *arena, usize size);

#endif /* MEMORY_H */

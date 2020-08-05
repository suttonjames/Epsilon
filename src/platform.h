#ifndef PLATFORM_H
#define PLATFORM_H

#include "language_layer.h"

typedef struct Button {
    b32 pressed;
} Button;

typedef struct Platform {
    usize permanent_arena_size;
    void *permanent_arena;

    b32 running;
    b32 initialised;

    s32 width, height;

    Vector2 mouse_position;
    Button keys[256];

    void *(*load_opengl_function)(char *name);
    void (*swap_buffers)(void);
} Platform;

#endif /* PLATFORM_H */

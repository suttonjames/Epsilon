#ifndef PLATFORM_H
#define PLATFORM_H

#include "language_layer.h"

typedef enum KeyCode {
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,

    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,

    MAX_KEYS
} KeyCode;

typedef enum KeyModifiers {
    KEY_MOD_CTRL,
    KEY_MOD_SHIFT,
    KEY_MOD_ALT
} KeyModifiers;

typedef enum MouseButtonCode {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE,

    MAX_BUTTONS
} MouseButtonCode;

typedef struct Key {
    KeyCode scan_code;
    KeyModifiers modifiers;
    b32 is_pressed;
    b32 is_repeat;
} Key;

typedef struct MouseButton {
    MouseButtonCode button;
    b32 is_pressed;
} MouseButton;

typedef enum EventType {
    EVENT_NONE,

    EVENT_KEY_PRESS,
    EVENT_KEY_RELEASE,

    EVENT_MOUSE_PRESS,
    EVENT_MOUSE_RELEASE,
    EVENT_MOUSE_MOVE,
    EVENT_MOUSE_SCROLL,

    EVENT_WINDOW_RESIZE
} EventType;

typedef struct Event {
    EventType type;
    Key key;
    MouseButton button;
    Vector2 position;
    Vector2 delta;
    Vector2 wheel;
} Event;

typedef struct InputState {
    b32 key_pressed[MAX_KEYS];
    b32 button_pressed[MAX_BUTTONS];
    Vector2 position;
} InputState;

typedef struct Platform {
    usize permanent_arena_size;
    void *permanent_arena;

    b32 running;
    b32 initialised;

    s32 width, height;

    InputState input;
    u32 event_count;
    Event events[1024];

    f32 frames_per_second;

    u64 ticks_per_second;
    u64 start_ticks;
    u64 end_ticks;

    void *(*load_opengl_function)(char *name);
    void (*swap_buffers)(void);
} Platform;

#endif /* PLATFORM_H */

#include "platform.h"

static Event key_press_event(KeyCode key, KeyModifiers mod, b32 is_repeat)
{
    Event event = { 0 };
    event.type = EVENT_KEY_PRESS;
    event.key.scan_code = key;
    event.key.modifiers = mod;
    event.key.is_pressed = true;
    event.key.is_repeat = is_repeat;

    return event;
}

static Event key_release_event(KeyCode key, KeyModifiers mod)
{
    Event event = { 0 };
    event.type = EVENT_KEY_RELEASE;
    event.key.scan_code = key;
    event.key.modifiers = mod;
    event.key.is_pressed = false;
    event.key.is_repeat = false;

    return event;
}

static Event mouse_press_event(MouseButtonCode button, Vector2 position)
{
    Event event = { 0 };
    event.type = EVENT_MOUSE_PRESS;
    event.button.button = button;
    event.button.is_pressed = true;
    event.position = position;

    return event;
}

static Event mouse_release_event(MouseButtonCode button, Vector2 position)
{
    Event event = { 0 };
    event.type = EVENT_MOUSE_RELEASE;
    event.button.button = button;
    event.button.is_pressed = false;
    event.position = position;

    return event;
}

static Event mouse_scroll_event(Vector2 wheel_delta)
{
    Event event = { 0 };
    event.type = EVENT_MOUSE_SCROLL;
    event.wheel = wheel_delta;

    return event;
}

static Event mouse_move_event(Vector2 position, Vector2 delta)
{
    Event event = { 0 };
    event.type = EVENT_MOUSE_MOVE;
    event.position = position;
    event.delta = delta;

    return event;
}

static void window_resize_event(Vector2 dimensions, Vector2 delta)
{
    //glviewport;
}

static void push_event_to_queue(Platform *platform, Event event)
{
    assert(event.type != EVENT_NONE);
    if (platform->event_count < 1024)
        platform->events[platform->event_count++] = event;
}

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "platform.h"

#include "opengl_win32.c"

static Platform platform;
static HDC device_context;

#define INIT_GAME(name) void name(Platform *platform)
typedef INIT_GAME(InitGame);

#define UPDATE_GAME(name) void name(Platform *platform)
typedef UPDATE_GAME(UpdateGame);

#define SHUTDOWN_GAME(name) void name(void)
typedef SHUTDOWN_GAME(ShutdownGame);

typedef struct GameCode {
    HMODULE dll;
    FILETIME last_write_time;

    InitGame *init_game;
    UpdateGame *update_game;
    ShutdownGame *shutdown_game;

} GameCode;

static char *dll_file_name = "c:/dev/epsilon/bin/epsilon.dll"; // temp
static char *temp_file_name = "c:/dev/epsilon/bin/epsilon_temp.dll"; // temp

static FILETIME win32_get_last_write_to_dll()
{
    FILETIME last_write_time = { 0 };
    WIN32_FIND_DATAA find_data;
    HANDLE file_handle = FindFirstFileA(dll_file_name, &find_data);
    if (file_handle != INVALID_HANDLE_VALUE) {
        last_write_time = find_data.ftLastWriteTime;
        FindClose(file_handle);
    }

    return last_write_time;
}

static GameCode win32_load_dll()
{
    GameCode game_code = { 0 };

    game_code.last_write_time = win32_get_last_write_to_dll();

    CopyFileA(dll_file_name, temp_file_name, FALSE);
    game_code.dll = LoadLibraryA(temp_file_name);
    if (game_code.dll) {
        game_code.init_game = (InitGame *)GetProcAddress(game_code.dll, "init_game");
        game_code.update_game = (UpdateGame *)GetProcAddress(game_code.dll, "update_game");
        game_code.shutdown_game = (ShutdownGame *)GetProcAddress(game_code.dll, "shutdown_game");
    } else {
        game_code.init_game = 0;
        game_code.update_game = 0;
        game_code.shutdown_game = 0;
    }

    return game_code;
}

static void win32_unload_dll(GameCode *game_code)
{
    if (game_code->dll) {
        FreeLibrary(game_code->dll);
        game_code->dll = 0;
    }
    game_code->init_game = 0;
    game_code->update_game = 0;
    game_code->shutdown_game = 0;
}

static Vector2 win32_get_mouse_position(HWND window)
{
    Vector2 result = { 0 };
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(window, &mouse);
    result.x = (f32)(mouse.x);
    result.y = (f32)(mouse.y);
    return result;
}

static void win32_init_timer()
{
    LARGE_INTEGER counter;
    QueryPerformanceFrequency(&counter);
    platform.ticks_per_second = counter.QuadPart;
}

static void win32_timer_start_frame()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    platform.start_ticks = counter.QuadPart;
}

static void win32_timer_end_frame(HWND window)
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    platform.end_ticks = counter.QuadPart;

    u64 elapsed_ticks = platform.end_ticks - platform.start_ticks;
    u64 desired_ticks = (u64)(platform.ticks_per_second / platform.frames_per_second);
    s64 ticks_to_wait = (s64)(desired_ticks - elapsed_ticks);

    LARGE_INTEGER wait_start;
    LARGE_INTEGER wait_end;

    QueryPerformanceCounter(&wait_start);

    while (ticks_to_wait > 0) {
        DWORD sleep_ms = (DWORD)(1000.0f * ((f64)ticks_to_wait / platform.ticks_per_second));
        if (sleep_ms > 0)
            Sleep(sleep_ms);

        QueryPerformanceCounter(&wait_end);
        ticks_to_wait -= wait_end.QuadPart - wait_start.QuadPart;
        wait_start = wait_end;
    }

    f32 ms_f = (f32)(1000.0f * ((f64)elapsed_ticks / platform.ticks_per_second));
    char title[32];
    sprintf(title, "Epsilon Engine: %f", ms_f);
    SetWindowTextA(window, title);
}

static void win32_swap_buffers(void)
{
    //SwapBuffers(device_context);
    wglSwapLayerBuffers(device_context, WGL_SWAP_MAIN_PLANE);
}

LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = -1;

    switch (message) {
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_QUIT:
            platform.running = false;
            PostQuitMessage(0);
            break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
            u64 key_code = wParam;
            b32 was_down = !!(lParam & (1 << 30));
            b32 is_down =   !(lParam & (1 << 31));

            if (is_down)
                platform.keys[key_code].pressed = true;
            else
                platform.keys[key_code].pressed = false;
            break;
        case WM_MOUSEMOVE:
            platform.mouse_position = win32_get_mouse_position(window);
            break;
        case WM_SIZE:
            platform.width = LOWORD(lParam);
            platform.height = HIWORD(lParam);
            break;
        default:
            result = DefWindowProc(window, message, wParam, lParam);
    }

    return result;
}

int main()
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HINSTANCE hInstance = GetModuleHandle(0);
    platform.running = true;

    win32_init_timer();

    platform.permanent_arena_size = gigabytes(1);
    platform.permanent_arena = VirtualAlloc(0, platform.permanent_arena_size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    WNDCLASSEXA window_class = { 0 };

    window_class.cbSize = sizeof(WNDCLASSEXA);
    window_class.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = "Win32Window";

    RegisterClassExA(&window_class);

    HWND window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "Epsilon Engine",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        0,
        hInstance,
        0);

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    device_context = GetDC(window);

    platform.frames_per_second = 60.0f;
    platform.load_opengl_function = win32_load_opengl_function;
    platform.swap_buffers = win32_swap_buffers;

    init_opengl(device_context);

    GameCode game_code = win32_load_dll();

    game_code.init_game(&platform);

    while (platform.running) {
        win32_timer_start_frame();

        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        FILETIME dll_write_time = win32_get_last_write_to_dll();

        if (CompareFileTime(&dll_write_time, &game_code.last_write_time) != 0) {
            win32_unload_dll(&game_code);
            game_code = win32_load_dll();
        }

        game_code.update_game(&platform);

        win32_timer_end_frame(window); // passing window is temp just to dispaly ms_f
    }

    game_code.shutdown_game();
    return 0;
}

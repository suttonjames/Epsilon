#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <assert.h>

#include "platform.h"

Platform platform;

#define INIT_GAME(name) void name(HDC device_context, Platform *platform)
typedef INIT_GAME(InitGame);

#define UPDATE_GAME(name) void name(HDC device_context, Platform *platform)
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
        default:
            result = DefWindowProc(window, message, wParam, lParam);
    }

    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    platform.running = true;
    platform.permanent_arena_size = gigabytes(1);
    platform.permanent_arena = VirtualAlloc(0, platform.permanent_arena_size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    WNDCLASSA window_class = { 0 };

    window_class.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
    window_class.lpfnWndProc = window_proc;
    window_class.hInstance = hInstance;
    window_class.lpszClassName = "Win32Window";

    RegisterClassA(&window_class);

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

    HDC device_context = GetDC(window);

    GameCode game_code = win32_load_dll();

    game_code.init_game(device_context, &platform);

    while (platform.running) {
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

        game_code.update_game(device_context, &platform);
    }

    game_code.shutdown_game();
    return 0;
}

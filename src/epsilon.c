#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "language_layer.h"
#include "opengl.c"

static HDC hdc = 0; // temp

__declspec(dllexport) void init_game(HDC device_context)
{
	hdc = device_context; // temp
	HGLRC rendering_context = init_opengl(device_context);

	draw_first_triangle();
}

__declspec(dllexport) void update_game()
{
	glClearColor(0.7f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

	SwapBuffers(hdc);
	//wglSwapLayerBuffers(device_context, WGL_SWAP_MAIN_PLANE);
}

__declspec(dllexport) void shutdown_game()
{

}
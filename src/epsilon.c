#include "platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "memory.c"
#include "opengl.c"
#include "mesh.c"

#include "epsilon.h"

GameState *game_state;

static HDC hdc = 0; // temp

__declspec(dllexport) void init_game(HDC device_context, Platform *platform)
{
    game_state = (GameState *)platform->permanent_arena;
    if (game_state) platform->initialised = true;

    alloc_arena(&game_state->assets, platform->permanent_arena_size - sizeof(game_state), (u64 *)platform->permanent_arena + sizeof(game_state));

    HGLRC rendering_context = init_opengl(device_context);

    const GLchar *vertex_source =
        "#version 330 core\n\
        layout(location = 0) in vec3 a_Position;\n\
        void main()\n\
        {\n\
            gl_Position = vec4(a_Position, 1.0);\n\
        }";

    const GLchar *fragment_source =
        "#version 330 core\n\
        out vec4 colour;\n\
        void main()\n\
        {\n\
            colour = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
        }";

    game_state->mesh = load_mesh(&game_state->assets, "c:/dev/epsilon/assets/helmet.obj"); // temp
    game_state->mesh->shader = load_shader(&game_state->assets, vertex_source, fragment_source);
}

__declspec(dllexport) void update_game(HDC device_context, Platform *platform)
{
    if (!game_state) {
        game_state = (GameState *)platform->permanent_arena;
        win32_load_opengl_functions();
    }
    hdc = device_context; // temp
    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(game_state->mesh->vertex_array);
    glUseProgram(game_state->mesh->shader->id);
    glDrawElements(GL_TRIANGLES, game_state->mesh->num_indices, GL_UNSIGNED_INT, 0);

    SwapBuffers(hdc);
    //wglSwapLayerBuffers(device_context, WGL_SWAP_MAIN_PLANE);
}

__declspec(dllexport) void shutdown_game()
{

}

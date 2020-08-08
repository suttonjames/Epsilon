#include "platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "memory.c"
#include "opengl.c"
#include "mesh.c"
#include "camera.c"

#include "epsilon.h"

GameState *game_state;

static f32 rotate_speed = 0.0f;

static void handle_events(Platform *platform)
{
    for (u32 i = 0; i < platform->event_count; i++) {
        // handle input events
    }
    platform->event_count = 0;
}

__declspec(dllexport) void init_game(Platform *platform)
{
    load_opengl_functions(platform);

    game_state = (GameState *)platform->permanent_arena;
    if (game_state) platform->initialised = true;

    alloc_arena(&game_state->assets, platform->permanent_arena_size - sizeof(game_state), (u64 *)platform->permanent_arena + sizeof(game_state));

    const GLchar *vertex_source =
        "#version 330 core\n\
        layout(location = 0) in vec3 a_Position;\n\
        layout(location = 1) in vec2 a_TexCoord;\n\
        uniform mat4 u_Model;\n\
        uniform mat4 u_View;\n\
        uniform mat4 u_Projection;\n\
        out vec2 v_TexCoord;\n\
        void main()\n\
        {\n\
            gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);\n\
            v_TexCoord = a_TexCoord;\n\
        }";

    const GLchar *fragment_source =
        "#version 330 core\n\
        in vec2 v_TexCoord;\n\
        uniform sampler2D u_Texture;\n\
        out vec4 colour;\n\
        void main()\n\
        {\n\
            //colour = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
            colour = texture(u_Texture, v_TexCoord);\n\
        }";

    game_state->mesh = load_mesh(&game_state->assets, "c:/dev/epsilon/assets/helmet.obj"); // temp
    game_state->mesh->shader = load_shader(&game_state->assets, vertex_source, fragment_source);
    game_state->mesh->texture = load_texture(&game_state->assets, "c:/dev/epsilon/assets/helmet_basecolor.tga");

    Matrix4x4 model = mat4(1.0f);//mat4_rotate(to_radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
    //Matrix4x4 view = mat4_translate(vec3(0.0f, 0.0f, -3.0f));
    Matrix4x4 projection = mat4_perspective(to_radians(45.0f), (f32)(platform->width / platform->height), 0.1f, 100.0f);
    game_state->camera = init_camera(&game_state->assets, projection);

    set_uniform_mat4(game_state->mesh->shader->id, "u_Model", model);
    set_uniform_mat4(game_state->mesh->shader->id, "u_View", game_state->camera->view_matrix);
    set_uniform_mat4(game_state->mesh->shader->id, "u_Projection", projection);
}

__declspec(dllexport) void update_game(Platform *platform)
{
    if (!game_state) {
        game_state = (GameState *)platform->permanent_arena;
        load_opengl_functions(platform);
    }

    handle_events(platform);

    glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, platform->width, platform->height);

    update_camera(game_state->camera, &platform->input, platform->width, platform->height);

    rotate_speed += 0.01f;
    Matrix4x4 trans = mat4(1.0f);
    //trans = mat4_mul(trans, mat4_scale(vec3(0.5f, 0.5f, 0.5f)));
    trans = mat4_mul(trans, mat4_rotate(rotate_speed, vec3(-1.0f, 1.0f, 0.0f)));

    //set_uniform_mat4(game_state->mesh->shader->id, "u_Model", trans);
    set_uniform_mat4(game_state->mesh->shader->id, "u_View", game_state->camera->view_matrix);
    set_uniform_mat4(game_state->mesh->shader->id, "u_Projection", game_state->camera->projection_matrix);

    glBindVertexArray(game_state->mesh->vertex_array);
    glBindTexture(GL_TEXTURE_2D, game_state->mesh->texture->id);
    glUseProgram(game_state->mesh->shader->id);
    glDrawElements(GL_TRIANGLES, game_state->mesh->num_indices, GL_UNSIGNED_INT, 0);

    platform->swap_buffers();
}

__declspec(dllexport) void shutdown_game()
{

}

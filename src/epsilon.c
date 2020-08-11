#include "platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "memory.c"
#include "opengl.c"
#include "mesh.c"
#include "camera.c"

#include "epsilon.h"

static GameState *game_state;

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

    game_state->mesh = load_mesh_from_file(&game_state->assets, "../assets/helmet.obj"); // temp
    game_state->mesh->shader = load_shader_from_file(&game_state->assets, "../assets/lighting_vertex.glsl", "../assets/lighting_fragment.glsl");
    game_state->mesh->texture = load_texture(&game_state->assets, "c:/dev/epsilon/assets/helmet_basecolor.tga");

    game_state->sky_box = create_skybox(&game_state->assets, "../assets/skybox/");

    game_state->box = load_cube(&game_state->assets);
    game_state->box->shader = game_state->mesh->shader; // have a shader library instead
    game_state->box->texture = load_texture(&game_state->assets, "../assets/container.jpg");

    game_state->sphere = load_sphere(&game_state->assets);
    game_state->sphere->shader = game_state->mesh->shader; // have a shader libary instead
    game_state->sphere->texture = load_texture(&game_state->assets, "../assets/basic_diff.tga");

    Matrix4x4 projection = mat4_perspective(to_radians(45.0f), (f32)(platform->width / platform->height), 0.1f, 100.0f);
    game_state->camera = init_camera(&game_state->assets, projection);
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

    // render model
    rotate_speed += 0.01f;
    Matrix4x4 trans = mat4_translate(vec3(0.0f, 0.0f, 0.0f));
    //trans = mat4_mul(trans, mat4_scale(vec3(0.5f, 0.5f, 0.5f)));
    trans = mat4_mul(trans, mat4_rotate(rotate_speed, vec3(-1.0f, 0.0f, 0.0f)));

    glUseProgram(game_state->mesh->shader->id);
    set_uniform_mat4(game_state->mesh->shader->id, "u_Model", trans);
    set_uniform_mat4(game_state->mesh->shader->id, "u_View", game_state->camera->view_matrix);
    set_uniform_mat4(game_state->mesh->shader->id, "u_Projection", game_state->camera->projection_matrix);
    set_uniform_vec3(game_state->mesh->shader->id, "u_LightColour", vec3(1.0f, 1.0f, 1.0f));
    set_uniform_vec3(game_state->mesh->shader->id, "u_LightPosition", vec3(1.0f, 1.0f, 1.0f));
    set_uniform_vec3(game_state->mesh->shader->id, "u_CameraPosition", game_state->camera->position);

    glBindTexture(GL_TEXTURE_2D, game_state->mesh->texture->id);
    glBindVertexArray(game_state->mesh->vertex_array);
    glDrawElements(GL_TRIANGLES, game_state->mesh->num_indices, GL_UNSIGNED_INT, 0);

    trans = mat4_translate(vec3(3.0f, 0.0f, 0.0f));
    set_uniform_mat4(game_state->box->shader->id, "u_Model", trans);

    glBindTexture(GL_TEXTURE_2D, game_state->box->texture->id);
    glBindVertexArray(game_state->box->vertex_array);
    glDrawElements(GL_TRIANGLES, game_state->box->num_indices, GL_UNSIGNED_INT, 0);

    trans = mat4_translate(vec3(-3.0f, 0.0f, 0.0f));
    set_uniform_mat4(game_state->sphere->shader->id, "u_Model", trans);

    glBindTexture(GL_TEXTURE_2D, game_state->sphere->texture->id);
    glBindVertexArray(game_state->sphere->vertex_array);
    glDrawElements(GL_TRIANGLES, game_state->sphere->num_indices, GL_UNSIGNED_INT, 0);

    // render skybox
    glDepthFunc(GL_LEQUAL);

    Matrix4x4 view = mat4_from_mat3(mat3(game_state->camera->view_matrix));

    glUseProgram(game_state->sky_box->shader->id);
    set_uniform_mat4(game_state->sky_box->shader->id, "u_View", view);
    set_uniform_mat4(game_state->sky_box->shader->id, "u_Projection", game_state->camera->projection_matrix);

    glBindTexture(GL_TEXTURE_CUBE_MAP, game_state->sky_box->texture->id);
    glBindVertexArray(game_state->sky_box->vertex_array);
    glDrawElements(GL_TRIANGLES, game_state->sky_box->num_indices, GL_UNSIGNED_INT, 0);

    glDepthFunc(GL_LESS);

    platform->swap_buffers();
}

__declspec(dllexport) void shutdown_game()
{

}

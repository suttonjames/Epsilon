#include "platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "memory.h"
#include "opengl.h"
#include "mesh.h"
#include "camera.h"

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

    game_state->sky_box = create_skybox(&game_state->assets, "../assets/textures/environment.hdr");

    game_state->model = load_mesh_from_file(&game_state->assets, "../assets/meshes/cerberus/cerberus.obj");
    game_state->model->shader = load_shader_from_file(&game_state->assets, "../assets/shaders/pbr_vertex.glsl", "../assets/shaders/pbr_fragment.glsl");
    game_state->model->material = push_struct(&game_state->assets, Material);
    game_state->model->material->albedo = load_texture(&game_state->assets, "../assets/meshes/cerberus/cerberus_A.tga");
    game_state->model->material->normal = load_texture(&game_state->assets, "../assets/meshes/cerberus/cerberus_N.tga");
    game_state->model->material->metalness = load_texture(&game_state->assets, "../assets/meshes/cerberus/cerberus_M.tga");
    game_state->model->material->roughness = load_texture(&game_state->assets, "../assets/meshes/cerberus/cerberus_R.tga");

    // enviroment textures
    game_state->irradiance = generate_texture_irradiance(&game_state->assets, game_state->sky_box->texture);
    game_state->prefilter = generate_texture_prefilter(&game_state->assets, game_state->sky_box->texture);
    game_state->brdf = generate_texture_brdf(&game_state->assets);

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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, platform->width, platform->height);

    update_camera(game_state->camera, &platform->input, platform->width, platform->height);

    // render models
    rotate_speed += 0.01f;
    Matrix4x4 trans = mat4_translate(vec3(0.0f, 0.0f, 0.0f));
    //trans = mat4_mul(trans, mat4_scale(vec3(0.5f, 0.5f, 0.5f)));
    trans = mat4_mul(trans, mat4_rotate(rotate_speed, vec3(0.0f, 1.0f, 0.0f)));

    glUseProgram(game_state->model->shader->id);
    set_uniform_mat4(game_state->model->shader->id, "model", trans);
    set_uniform_mat4(game_state->model->shader->id, "view", game_state->camera->view_matrix);
    set_uniform_mat4(game_state->model->shader->id, "projection", game_state->camera->projection_matrix);

    set_uniform_vec3(game_state->model->shader->id, "light.direction", vec3(1.0f, 0.0f, 1.0f));
    set_uniform_vec3(game_state->model->shader->id, "light.radiance", vec3(0.5f, 0.5f, 0.5f));

    set_uniform_int(game_state->model->shader->id, "albedo_texture", 0);
    set_uniform_int(game_state->model->shader->id, "normal_texture", 1);
    set_uniform_int(game_state->model->shader->id, "metalness_texture", 2);
    set_uniform_int(game_state->model->shader->id, "roughness_texture", 3);
    set_uniform_int(game_state->model->shader->id, "irradiance_map", 4);
    set_uniform_int(game_state->model->shader->id, "prefilter_map", 5);
    set_uniform_int(game_state->model->shader->id, "brdf_lut_map", 6);

    set_uniform_vec3(game_state->model->shader->id, "camera_position", game_state->camera->position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, game_state->model->material->albedo->id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, game_state->model->material->normal->id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, game_state->model->material->metalness->id);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, game_state->model->material->roughness->id);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, game_state->irradiance->id);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, game_state->prefilter->id);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, game_state->brdf->id);
    glBindVertexArray(game_state->model->vertex_array);
    glDrawElements(GL_TRIANGLES, game_state->model->num_indices, GL_UNSIGNED_INT, 0);

    // render skybox
    glDepthFunc(GL_LEQUAL);

    Matrix4x4 view = mat4_from_mat3(mat3(game_state->camera->view_matrix));

    glUseProgram(game_state->sky_box->shader->id);
    set_uniform_mat4(game_state->sky_box->shader->id, "view", view);
    set_uniform_mat4(game_state->sky_box->shader->id, "projection", game_state->camera->projection_matrix);

    glBindTexture(GL_TEXTURE_CUBE_MAP, game_state->sky_box->texture->id);
    glBindVertexArray(game_state->sky_box->vertex_array);
    glDrawElements(GL_TRIANGLES, game_state->sky_box->num_indices, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    glDepthFunc(GL_LESS);

    platform->swap_buffers();
}

__declspec(dllexport) void shutdown_game()
{

}

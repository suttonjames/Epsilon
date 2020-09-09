#include "opengl.h"

Shader *load_shader(MemoryArena *arena, const char *vertex_source, const char *fragment_source)
{
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint success;
    GLchar info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("%s\n", info_log);
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("%s\n", info_log);
        glGetProgramInfoLog(program, 512, NULL, info_log);
        printf("%s\n", info_log);
        assert(false);
        // log errors
    }

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    Shader *shader = push_struct(arena, Shader);
    shader->id = program;

    return shader;
}

Shader *load_shader_from_file(MemoryArena *arena, const char *vertex_file, const char *fragment_file)
{
    char *vertex_source = read_file(vertex_file);
    char *fragment_source = read_file(fragment_file);

    Shader *shader = load_shader(arena, vertex_source, fragment_source);

    free(vertex_source);
    free(fragment_source);

    return shader;
}

void set_uniform_int(GLuint shader_id, const char *name, s32 value)
{
    GLuint location = glGetUniformLocation(shader_id, name);
    glUniform1i(location, value);
}

void set_uniform_float(GLuint shader_id, const char *name, f32 value)
{
    GLuint location = glGetUniformLocation(shader_id, name);
    glUniform1f(location, value);
}

void set_uniform_vec2(GLuint shader_id, const char *name, Vector2 v)
{
    GLuint location = glGetUniformLocation(shader_id, name);
    glUniform2f(location, v.x, v.y);
}

void set_uniform_vec3(GLuint shader_id, const char *name, Vector3 v)
{
    GLuint location = glGetUniformLocation(shader_id, name);
    glUniform3f(location, v.x, v.y, v.z);
}

void set_uniform_vec4(GLuint shader_id, const char *name, Vector4 v)
{
    GLuint location = glGetUniformLocation(shader_id, name);
    glUniform4f(location, v.x, v.y, v.z, v.w);
}

void set_uniform_mat3(GLuint shader_id, const char *name, Matrix3x3 m)
{
    GLuint location = glGetUniformLocation(shader_id, name);
    glUniformMatrix3fv(location, 1, GL_FALSE, m.item);
}

void set_uniform_mat4(GLuint shader_id, const char *name, Matrix4x4 m)
{
    GLuint location = glGetUniformLocation(shader_id, name); // temp, store uniform locations when setting up shader
    glUniformMatrix4fv(location, 1, GL_FALSE, m.item);
}

Texture *load_texture(MemoryArena *arena, const char *file_name)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (stbi_is_hdr(file_name)) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        s32 width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        u8 *data = (u8*)stbi_loadf(file_name, &width, &height, &channels, 0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGB, GL_FLOAT, data);
        stbi_image_free(data);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        s32 width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        u8 *data = stbi_load(file_name, &width, &height, &channels, 0);
        if (data) {
            if (channels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else if (channels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            assert(false);
            // log error
        }
        stbi_image_free(data);
    }

    Texture *texture = push_struct(arena, Texture);
    texture->id = id;

    return texture;
}

Texture *load_cubemap(MemoryArena *arena, const char *file_name)
{
    // sort this out
    const char *cube_map[6];
    cube_map[0] = "../assets/skybox/right.jpg";
    cube_map[1] = "../assets/skybox/left.jpg";
    cube_map[2] = "../assets/skybox/top.jpg";
    cube_map[3] = "../assets/skybox/bottom.jpg";
    cube_map[4] = "../assets/skybox/front.jpg";
    cube_map[5] = "../assets/skybox/back.jpg";

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    s32 width, height, channels;
    stbi_set_flip_vertically_on_load(false);

    for (u32 i = 0; i < 6; i++) {
        u8 *data = stbi_load(cube_map[i], &width, &height, &channels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            assert(false);
            // log error
        }
        stbi_image_free(data);
    }

    Texture *texture = push_struct(arena, Texture);
    texture->id = id;

    return texture;
}

Texture *generate_texture_cubemap(MemoryArena *arena, const char *file_name)
{
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    s32 size = 512;

    GLuint framebuffer, renderbuffer;
    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    Texture *cubemap = push_struct(arena, Texture);
    glGenTextures(1, &cubemap->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);
    for (u32 i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, size, size, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Matrix4x4 framebuffer_projection = mat4_perspective(to_radians(90.0f), 1.0, 0.1f, 100.0f);
    Matrix4x4 framebuffer_view[6] = {
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))
    };

    Shader *shader = load_shader_from_file(arena, "../assets/shaders/cubemap_vertex.glsl", "../assets/shaders/cubemap_fragment.glsl");
    Texture *texture = load_texture(arena, file_name);

    glUseProgram(shader->id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    set_uniform_mat4(shader->id, "projection", framebuffer_projection);

    glViewport(0, 0, size, size);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    for (s32 i = 0; i < 6; i++) {
        set_uniform_mat4(shader->id, "view", framebuffer_view[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap->id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Mesh *cube = load_cube(arena);
        glBindVertexArray(cube->vertex_array);
        glDrawElements(GL_TRIANGLES, cube->num_indices, GL_UNSIGNED_INT, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return cubemap;
}

Texture *generate_texture_irradiance(MemoryArena *arena, Texture *cubemap)
{
    s32 size = 32;

    GLuint framebuffer, renderbuffer;
    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    Texture *irradiance = push_struct(arena, Texture);
    glGenTextures(1, &irradiance->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance->id);
    for (u32 i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Matrix4x4 framebuffer_projection = mat4_perspective(to_radians(90.0f), 1.0, 0.1f, 100.0f);
    Matrix4x4 framebuffer_view[6] = {
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))
    };

    Shader *shader = load_shader_from_file(arena, "../assets/shaders/irradiance_vertex.glsl", "../assets/shaders/irradiance_fragment.glsl");

    glUseProgram(shader->id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);
    set_uniform_mat4(shader->id, "projection", framebuffer_projection);

    glViewport(0, 0, size, size);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    for (s32 i = 0; i < 6; i++) {
        set_uniform_mat4(shader->id, "view", framebuffer_view[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance->id, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Mesh *cube = load_cube(arena);
        glBindVertexArray(cube->vertex_array);
        glDrawElements(GL_TRIANGLES, cube->num_indices, GL_UNSIGNED_INT, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return irradiance;
}

Texture *generate_texture_prefilter(MemoryArena *arena, Texture *cubemap)
{
    s32 size = 256;

    GLuint framebuffer, renderbuffer;
    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    Texture *prefilter = push_struct(arena, Texture);
    glGenTextures(1, &prefilter->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter->id);
    for (u32 i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, size, size, 0, GL_RGB, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    Matrix4x4 framebuffer_projection = mat4_perspective(to_radians(90.0f), 1.0, 0.1f, 100.0f);
    Matrix4x4 framebuffer_view[6] = {
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
        mat4_lookat(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))
    };

    Shader *shader = load_shader_from_file(arena, "../assets/shaders/prefilter_vertex.glsl", "../assets/shaders/prefilter_fragment.glsl");

    glUseProgram(shader->id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id);
    set_uniform_mat4(shader->id, "projection", framebuffer_projection);

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    s32 MAX_MIPMAP_LEVELS = 5;

    for (s32 mip = 0; mip < MAX_MIPMAP_LEVELS; mip++) {
        u32 mip_width  = size*(s32)powf(0.5f, (f32)mip);
        u32 mip_height = size*(s32)powf(0.5f, (f32)mip);

        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mip_width, mip_height);
        glViewport(0, 0, mip_width, mip_height);

        f32 roughness = (f32)mip/(f32)(MAX_MIPMAP_LEVELS - 1);
        set_uniform_float(shader->id, "roughness", roughness);

        for (s32 i = 0; i < 6; i++) {
            set_uniform_mat4(shader->id, "view", framebuffer_view[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter->id, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Mesh *cube = load_cube(arena);
            glBindVertexArray(cube->vertex_array);
            glDrawElements(GL_TRIANGLES, cube->num_indices, GL_UNSIGNED_INT, 0);
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return prefilter;
}

Texture *generate_texture_brdf(MemoryArena *arena)
{
    s32 size = 512;

    Texture *brdf = push_struct(arena, Texture);
    glGenTextures(1, &brdf->id);
    glBindTexture(GL_TEXTURE_2D, brdf->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size, size, 0, GL_RGB, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint framebuffer, renderbuffer;
    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdf->id, 0);

    Shader *shader = load_shader_from_file(arena, "../assets/shaders/brdf_vertex.glsl", "../assets/shaders/brdf_fragment.glsl");

    glViewport(0, 0, size, size);
    glUseProgram(shader->id);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_quad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return brdf;
}

void generate_enviroment_maps(MemoryArena *arena, const char *file_name, Texture *cubemap, Texture *irradiance, Texture *prefilter, Texture *brdf)
{
}



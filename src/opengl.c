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
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        glGetProgramInfoLog(program, 512, NULL, info_log);
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

void set_uniform_mat4(GLuint shader_id, const char *name, Matrix4x4 m)
{
    GLuint location = glGetUniformLocation(shader_id, name); // temp store uniform locations when setting up shader
    glUniformMatrix4fv(location, 1, GL_FALSE, m.item);
}

Texture *load_texture(MemoryArena *arena, const char *file_name)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

    Texture *texture = push_struct(arena, Texture);
    texture->id = id;

    return texture;
}

TextureCube *load_cubemap(MemoryArena *arena, const char *file_name)
{
     //right, left, top, bottom, back, front
    const char *cube_map[6];
    cube_map[0] = "c:/dev/epsilon/assets/skybox/right.jpg";
    cube_map[1] = "c:/dev/epsilon/assets/skybox/left.jpg";
    cube_map[2] = "c:/dev/epsilon/assets/skybox/top.jpg";
    cube_map[3] = "c:/dev/epsilon/assets/skybox/bottom.jpg";
    cube_map[4] = "c:/dev/epsilon/assets/skybox/front.jpg";
    cube_map[5] = "c:/dev/epsilon/assets/skybox/back.jpg";

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

    TextureCube *texture_cube = push_struct(arena, TextureCube);
    texture_cube->texture = push_struct(arena, Texture);
    texture_cube->texture->id = id;

    f32 sky_box_verts[] = {
    // positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

    glGenVertexArrays(1, &texture_cube->vertex_array);
    glBindVertexArray(texture_cube->vertex_array);

    glGenBuffers(1, &texture_cube->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, texture_cube->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sky_box_verts), &sky_box_verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

    /*glGenBuffers(1, &texture_cube->index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture_cube->index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, texture_cube->num_indices * sizeof(u32), &texture_cube->indices[0], GL_STATIC_DRAW);*/

    return texture_cube;
}


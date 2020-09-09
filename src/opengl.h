#ifndef OPENGL_H
#define OPENGL_H

#include <gl/Gl.h>
#include "gl/wglext.h"
#include "gl/glext.h"

#define GLProc(name, proc) PFN##proc##PROC name = 0;
#include "opengl_functions.inc"

void load_opengl_functions(Platform *platform)
{
#define GLProc(name, proc) name = (PFN##proc##PROC)platform->load_opengl_function(#name);
#include "opengl_functions.inc"
}

typedef struct Shader {
    GLuint id;
} Shader;

typedef struct Texture {
    GLuint id;
} Texture;

Shader *load_shader(MemoryArena *arena, const char *vertex_source, const char *fragment_source);
Shader *load_shader_from_file(MemoryArena *arena, const char *vertex_file, const char *fragment_file);

void set_uniform_int(GLuint shader_id, const char *name, s32 value);
void set_uniform_float(GLuint shader_id, const char *name, f32 value);
void set_uniform_vec2(GLuint shader_id, const char *name, Vector2 v);
void set_uniform_vec3(GLuint shader_id, const char *name, Vector3 v);
void set_uniform_vec4(GLuint shader_id, const char *name, Vector4 v);
void set_uniform_mat3(GLuint shader_id, const char *name, Matrix3x3 m);
void set_uniform_mat4(GLuint shader_id, const char *name, Matrix4x4 m);

Texture *load_texture(MemoryArena *arena, const char *file_name);
Texture *load_cubemap(MemoryArena *arena, const char *file_name);
void generate_enviroment_maps(MemoryArena *arena, const char *file_name, Texture *cubemap, Texture *irradiance, Texture *prefilter, Texture *brdf);

#endif /* OPENGL_H */

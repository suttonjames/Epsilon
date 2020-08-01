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

void set_uniform_mat4(GLuint shader_id, const char *name, Matrix4x4 m);

Texture *load_texture(MemoryArena *arena, const char *file_name);

#endif /* OPENGL_H */

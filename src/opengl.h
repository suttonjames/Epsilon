#ifndef OPENGL_H
#define OPENGL_H

#include "opengl_win32.h"

#include "gl/glext.h"

extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLCREATEBUFFERSPROC glCreateBuffers;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

static HGLRC init_opengl(HDC device_context);
static void draw_first_triangle();

typedef struct Shader {
    GLuint id;
} Shader;

typedef struct Texture {
    GLuint id;
} Texture;

Shader *load_shader(MemoryArena *arena, const char *vertex_source, const char *fragment_source);

Texture *load_texture(MemoryArena *arena, const char *file_name);

#endif /* OPENGL_H */

#ifndef MESH_H
#define MESH_H

#include "stb/stretchy_buffer.h"

typedef struct Vertex {
    Vector3 position;
    Vector2 texcoord;
    Vector3 normal;
} Vertex;

typedef struct Mesh {
    Vertex *vertices;
    u32 num_vertices;

    u32 *indices;
    u32 num_indices;

    Shader *shader;
    Texture *texture;

    GLuint vertex_array, vertex_buffer, index_buffer;
} Mesh;

Mesh *load_mesh(MemoryArena *arena, const char *file_name);

#endif /* MESH_H */

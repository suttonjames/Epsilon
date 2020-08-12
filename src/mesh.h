#ifndef MESH_H
#define MESH_H

#include "stb/stretchy_buffer.h"

typedef struct Vertex {
    Vector3 position;
    Vector2 texcoord;
    Vector3 normal;
} Vertex;

typedef struct Material {
    Texture *diffuse_map;
    Texture *specular_map;
    Texture *emission_map;
} Material;

typedef struct Mesh {
    Vertex *vertices;
    u32 num_vertices;

    u32 *indices;
    u32 num_indices;

    Shader *shader;
    Material *material;

    GLuint vertex_array, vertex_buffer, index_buffer;
} Mesh;

Mesh *load_mesh_from_file(MemoryArena *arena, const char *file_name);
Mesh *create_skybox(MemoryArena *arena, const char *file_name);

Mesh *load_cube(MemoryArena *arena);
Mesh *load_sphere(MemoryArena *arena);

#endif /* MESH_H */

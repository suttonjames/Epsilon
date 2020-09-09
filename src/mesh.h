#ifndef MESH_H
#define MESH_H

#include "stb/stretchy_buffer.h"

typedef struct Vertex {
    Vector3 position;
    Vector2 texcoord;
    Vector3 normal;
} Vertex;

typedef struct Material {
    Texture *albedo;
    Texture *normal;
    Texture *metalness;
    Texture *roughness;
} Material;

typedef struct Mesh {
    Vertex *vertices;
    u32 num_vertices;

    u32 *indices;
    u32 num_indices;

    Shader *shader;
    Material *material;
    Texture *texture;

    GLuint vertex_array, vertex_buffer, index_buffer;
} Mesh;

Mesh *load_mesh_from_file(MemoryArena *arena, const char *file_name);
Mesh *create_skybox(MemoryArena *arena, const char *file_name);

Mesh *load_cube(MemoryArena *arena);
Mesh *load_sphere(MemoryArena *arena);
void draw_quad(void);

#endif /* MESH_H */

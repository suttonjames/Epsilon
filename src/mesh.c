#include "mesh.h"

static void upload_mesh(Mesh *mesh)
{
    glGenVertexArrays(1, &mesh->vertex_array);
    glBindVertexArray(mesh->vertex_array);

    glGenBuffers(1, &mesh->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(f32)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(f32)));

    glGenBuffers(1, &mesh->index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices * sizeof(u32), mesh->indices, GL_STATIC_DRAW);
}

static char *read_line(FILE *file)
{
    char *line = NULL;

    while (1) {
        s32 c = fgetc(file);
        if (c == EOF) return line;
        sb_push(line, (char)c);
        if (c == '\n') {
            sb_push(line, '\0');
            return line;
        }
    }
}

Mesh *load_mesh_from_file(MemoryArena *arena, const char *file_name)
{
    FILE *file;

    Vector3 *positions = NULL;
    Vector2 *texcoords = NULL;
    Vector3 *normals = NULL;
    s32 *position_index = NULL;
    s32 *texcoord_index = NULL;
    s32 *normal_index = NULL;

    file = fopen(file_name, "rb");
    assert(file != NULL);
    while (1) {
        char *line = read_line(file);
        if (line == NULL) break;

        if (strncmp(line, "v ", 2) == 0) { // positions
            Vector3 position;
            s32 items = sscanf(line + 2, "%f %f %f", &position.x, &position.y, &position.z);
            assert(items == 3);
            sb_push(positions, position);
        } else if (strncmp(line, "vt ", 3) == 0) { // texcoords
            Vector2 texcoord;
            s32 items = sscanf(line + 3, "%f %f", &texcoord.x, &texcoord.y);
            assert(items == 2);
            sb_push(texcoords, texcoord);
        } else if (strncmp(line, "vn ", 3) == 0) { // normals
            Vector3 normal;
            s32 items = sscanf(line + 3, "%f %f %f", &normal.x, &normal.y, &normal.z);
            assert(items == 3);
            sb_push(normals, normal);
        } else if (strncmp(line, "f ", 2) == 0) { // faces
            s32 position[3], texcoord[3], normal[3];
#if 1 // helmet
            s32 items = sscanf(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d",
                &position[0], &texcoord[0], &normal[0],
                &position[1], &texcoord[1], &normal[1],
                &position[2], &texcoord[2], &normal[2]
            );
#else // teapot
            s32 items = sscanf(line + 2, "%d %d %d", &position[0], &position[1], &position[2]);
#endif
            assert(items == 3 || items == 9);
            for (s32 i = 0; i < 3; i++) {
                sb_push(position_index, position[i] - 1);
                sb_push(texcoord_index, texcoord[i] - 1);
                sb_push(normal_index, normal[i] - 1);
            }
        }
        sb_free(line);
    }
    fclose(file);

    Mesh *mesh = push_struct(arena, Mesh);
    mesh->num_vertices = sb_count(position_index);
    mesh->num_indices = 3 * mesh->num_vertices;

    Vertex *vertices = NULL;
    u32 *indices = NULL;
    u32 offset = 0;
    for (u32 i = 0; i < mesh->num_vertices; i++) {
        Vertex vertex;
        vertex.position = positions[position_index[i]];
        vertex.texcoord = texcoords[texcoord_index[i]];
        vertex.normal = normals[normal_index[i]];
        sb_push(vertices, vertex);
        sb_push(indices, 0 + offset);
        sb_push(indices, 1 + offset);
        sb_push(indices, 2 + offset);
        offset += 3;
    }

    mesh->vertices = push_array(arena, mesh->num_vertices, Vertex);
    mesh->indices = push_array(arena, mesh->num_indices, u32);

    memcpy(mesh->vertices, vertices, sizeof(Vertex) * mesh->num_vertices);
    memcpy(mesh->indices, indices, sizeof(u32) * mesh->num_indices);

    upload_mesh(mesh);

    sb_free(positions);
    sb_free(texcoords);
    sb_free(normals);
    sb_free(position_index);
    sb_free(texcoord_index);
    sb_free(normal_index);

    sb_free(vertices);
    sb_free(indices);

    return mesh;
}

Mesh *create_skybox(MemoryArena *arena, const char *file_name)
{
    Mesh *sky_box = load_cube(arena);
    sky_box->texture = generate_texture_cubemap(arena, file_name);
    //sky_box->texture = load_cubemap(arena, file_name);
    sky_box->shader = load_shader_from_file(arena, "../assets/skybox_vertex.glsl", "../assets/skybox_fragment.glsl");

    return sky_box;
}

Mesh *load_cube(MemoryArena *arena)
{
    // temp! have some sort of mesh/asset libary so we dont reupload each time
    Mesh *mesh = load_mesh_from_file(arena, "../assets/cube.obj");
    return mesh;
}

Mesh *load_sphere(MemoryArena *arena)
{
    Mesh *mesh = load_mesh_from_file(arena, "../assets/sphere.obj");
    return mesh;
}

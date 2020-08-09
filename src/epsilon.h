#ifndef EPSILON_H
#define EPSILON_H

typedef struct GameState {
    MemoryArena assets;
    Mesh *mesh;
    TextureCube *sky_box;

    Camera *camera;
} GameState;

#endif /* EPSILON_H */

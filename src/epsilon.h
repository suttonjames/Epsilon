#ifndef EPSILON_H
#define EPSILON_H

typedef struct GameState {
    MemoryArena assets;

    Mesh *mesh;
    Mesh *sky_box;
    Mesh *box;
    Mesh *sphere;

    Camera *camera;
} GameState;

#endif /* EPSILON_H */

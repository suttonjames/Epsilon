#ifndef EPSILON_H
#define EPSILON_H

typedef struct GameState {
    MemoryArena assets;
    Mesh *mesh;

    Camera *camera;
} GameState;

#endif /* EPSILON_H */

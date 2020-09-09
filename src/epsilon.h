#ifndef EPSILON_H
#define EPSILON_H

typedef struct GameState {
    MemoryArena assets;

    Mesh *model;
    Mesh *sky_box;
    Mesh *box;
    Mesh *sphere;

    // Scene struct?
    Texture *irradiance;
    Texture *prefilter;
    Texture *brdf;

    Camera *camera;
} GameState;

#endif /* EPSILON_H */

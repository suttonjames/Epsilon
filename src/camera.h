#ifndef CAMERA_H
#define CAMERA_H

typedef struct Camera {
    Vector3 position;
    Quaternion orientation;

    Matrix4x4 view_matrix;
    Matrix4x4 projection_matrix;
} Camera;

Camera *init_camera(MemoryArena *arena, Matrix4x4 projection);
void update_camera(Camera *camera, Platform *platform);

#endif /* CAMERA_H */
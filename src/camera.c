#include "camera.h"

static Vector3 forward(Camera *camera)
{
    return quat_mul_vec3(quat_conjugate(camera->orientation), vec3(0.0f, 0.0f, -1.0f));
}

static Vector3 backward(Camera *camera)
{
    return quat_mul_vec3(quat_conjugate(camera->orientation), vec3(0.0f, 0.0f, 1.0f));
}

static Vector3 right(Camera *camera)
{
    return quat_mul_vec3(quat_conjugate(camera->orientation), vec3(1.0f, 0.0f, 0.0f));
}

Vector3 left(Camera *camera)
{
    return quat_mul_vec3(quat_conjugate(camera->orientation), vec3(-1.0f, 0.0f, 0.0f));
}

static Vector3 up(Camera *camera)
{
    return quat_mul_vec3(quat_conjugate(camera->orientation), vec3(0.0f, 1.0f, 0.0f));
}

static Vector3 down(Camera *camera)
{
    return quat_mul_vec3(quat_conjugate(camera->orientation), vec3(0.0f, -1.0f, 0.0f));
}

static void update_camera_view(Camera *camera)
{
    Matrix4x4 rotation = quat_to_mat4(camera->orientation);
    Matrix4x4 translation = mat4_translate(vec3(-camera->position.x, -camera->position.y, -camera->position.z));
    camera->view_matrix = mat4_mul(rotation, translation);
}

Camera *init_camera(MemoryArena *arena, Matrix4x4 projection)
{
    Camera *camera = push_struct(arena, Camera);
    camera->projection_matrix = projection;

    camera->position = vec3(0.0f, 0.0f, 5.0f);

    camera->view_matrix = mat4_lookat(camera->position, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    camera->orientation = mat4_to_quat(camera->view_matrix);

    update_camera_view(camera);

    return camera;
}

void update_camera(Camera *camera, Platform *platform)
{
    Vector3 velocity_dir = { 0 };
    f32 velocity_speed = 0.1f;

    if (platform->keys[87].pressed == true) // W
        velocity_dir = vec3_add(velocity_dir, forward(camera));
    if (platform->keys[83].pressed == true) // S
        velocity_dir = vec3_add(velocity_dir, backward(camera));

    if (platform->keys[65].pressed == true) // A
        velocity_dir = vec3_add(velocity_dir, left(camera));
    if (platform->keys[68].pressed == true) // D
        velocity_dir = vec3_add(velocity_dir, right(camera));

    if (vec3_length(velocity_dir) > 0)
        velocity_dir = vec3_norm(velocity_dir);

    camera->position = vec3_add(camera->position, vec3_mul_float(velocity_dir, velocity_speed));

    Vector2 delta = vec2(0.0f, 0.0f);
    delta.x = platform->mouse_position.x - platform->width / 2;
    delta.y = platform->mouse_position.y - platform->height / 2;

    Quaternion yaw = angle_axis(0.005f * to_radians(delta.x), vec3(0.0f, 1.0f, 0.0f));
    Quaternion pitch = angle_axis(0.005f * to_radians(delta.y), right(camera));

    Quaternion rotation = quat_mul(yaw, pitch);

    camera->orientation = quat_mul(camera->orientation, rotation);
    camera->orientation = quat_norm(camera->orientation);

    update_camera_view(camera);
}

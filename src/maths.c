#include "maths.h"

inline Matrix4x4 mat4(f32 f)
{
    Matrix4x4 result = { 0 };

    result.elements[0][0] = f;
    result.elements[1][1] = f;
    result.elements[2][2] = f;
    result.elements[3][3] = f;

    return result;
}

inline Matrix4x4 mat4_mul(Matrix4x4 a, Matrix4x4 b)
{
    Matrix4x4 result = { 0 };

    for (s32 j = 0; j < 4; ++j) {
        for (s32 i = 0; i < 4; ++i) {
            result.elements[i][j] = (a.elements[0][j] * b.elements[i][0] +
                a.elements[1][j] * b.elements[i][1] +
                a.elements[2][j] * b.elements[i][2] +
                a.elements[3][j] * b.elements[i][3]);
        }
    }

    return result;
}

inline Matrix4x4 mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z)
{
    Matrix4x4 result = mat4(0.0f);

    result.elements[0][0] = 2.0f / (right - left);
    result.elements[1][1] = 2.0f / (top - bottom);
    result.elements[2][2] = 2.0f / (near_z - far_z);
    result.elements[3][3] = 1.0f;

    result.elements[3][0] = (left + right) / (left - right);
    result.elements[3][1] = (bottom + top) / (bottom - top);
    result.elements[3][2] = (far_z + near_z) / (near_z - far_z);

    return result;
}


inline Matrix4x4 mat4_perspective(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z)
{
    Matrix4x4 result = mat4(0.0f);

    f32 tan_theta_over_2 = tanf(fov / 2.0f);

    result.elements[0][0] = 1.0f / (aspect_ratio * tan_theta_over_2);
    result.elements[1][1] = 1.0f / tan_theta_over_2;
    result.elements[2][3] = -1.0f;
    result.elements[2][2] = (near_z + far_z) / (near_z - far_z);
    result.elements[3][2] = (2.0f * near_z * far_z) / (near_z - far_z);

    return result;
}

inline Matrix4x4 mat4_translate(Vector3 translation)
{
    Matrix4x4 result = mat4(1.0f);

    result.elements[3][0] = translation.x;
    result.elements[3][1] = translation.y;
    result.elements[3][2] = translation.z;

    return result;
}

inline Matrix4x4 mat4_rotate(f32 angle, Vector3 axis)
{
    Matrix4x4 result = mat4(1.0f);

    axis = vec3_norm(axis);

    f32 sin_theta = sinf(angle);
    f32 cos_theta = cosf(angle);
    f32 one_minus_cos_theta = 1.0f - cos_theta;

    result.elements[0][0] = (axis.x * axis.x * one_minus_cos_theta) + cos_theta;
    result.elements[0][1] = (axis.x * axis.y * one_minus_cos_theta) + (axis.z * sin_theta);
    result.elements[0][2] = (axis.x * axis.z * one_minus_cos_theta) - (axis.y * sin_theta);

    result.elements[1][0] = (axis.y * axis.x * one_minus_cos_theta) - (axis.z * sin_theta);
    result.elements[1][1] = (axis.y * axis.y * one_minus_cos_theta) + cos_theta;
    result.elements[1][2] = (axis.y * axis.z * one_minus_cos_theta) + (axis.x * sin_theta);

    result.elements[2][0] = (axis.z * axis.x * one_minus_cos_theta) + (axis.y * sin_theta);
    result.elements[2][1] = (axis.z * axis.y * one_minus_cos_theta) - (axis.x * sin_theta);
    result.elements[2][2] = (axis.z * axis.z * one_minus_cos_theta) + cos_theta;

    return result;
}

inline Matrix4x4 mat4_scale(Vector3 scale)
{
    Matrix4x4 result = mat4(1.0f);

    result.elements[0][0] = scale.x;
    result.elements[1][1] = scale.y;
    result.elements[2][2] = scale.z;

    return result;
}

inline Matrix4x4 mat4_lookat(Vector3 eye, Vector3 centre, Vector3 up)
{
    Matrix4x4 result = mat4(1.0f);

    Vector3 z_axis = vec3_norm(vec3_sub(centre, eye));
    Vector3 x_axis = vec3_norm(vec3_cross(z_axis, up));
    Vector3 y_axis = vec3_cross(x_axis, z_axis);

    result.elements[0][0] = x_axis.x;
    result.elements[0][1] = y_axis.x;
    result.elements[0][2] = -z_axis.x;

    result.elements[1][0] = x_axis.y;
    result.elements[1][1] = y_axis.y;
    result.elements[1][2] = -z_axis.y;

    result.elements[2][0] = x_axis.z;
    result.elements[2][1] = y_axis.z;
    result.elements[2][2] = -z_axis.z;

    result.elements[3][0] = -vec3_dot(x_axis, eye);
    result.elements[3][1] = -vec3_dot(y_axis, eye);
    result.elements[3][2] = vec3_dot(z_axis, eye);

    return result;
}

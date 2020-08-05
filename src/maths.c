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

inline Matrix4x4 mat4_mul_float(Matrix4x4 m, f32 f)
{
    for (s32 j = 0; j < 4; ++j) {
        for (s32 i = 0; i < 4; ++i) {
            m.elements[i][j] *= f;
        }
    }
    return m;
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

inline Quaternion quat_mul(Quaternion a, Quaternion b)
{
    Quaternion result;

    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

    return result;
}

inline Vector3 quat_mul_vec3(Quaternion q, Vector3 v)
{
    Vector3 t = vec3_mul_float(vec3_cross(vec3(q.x, q.y, q.z), v), 2.0f);
    return vec3_add(vec3_add(v, vec3_mul_float(t, q.w)), vec3_cross(vec3(q.x, q.y, q.z), t));
}

inline Quaternion quat_conjugate(Quaternion q)
{
    Quaternion result;

    result.x = -q.x;
    result.y = -q.y;
    result.z = -q.z;
    result.w = q.w;

    return result;
}

inline Quaternion angle_axis(f32 angle, Vector3 axis)
{
    Quaternion result;

    axis = vec3_norm(axis);

    f32 s = (f32)sin(angle * 0.5f);

    result.x = axis.x * s;
    result.y = axis.y * s;
    result.z = axis.z * s;
    result.w = (f32)cos(angle * 0.5f);

    return result;
}

inline Matrix4x4 quat_to_mat4(Quaternion q)
{
    Matrix4x4 result = mat4(1.0f);

    f32 xx = q.x * q.x;
    f32 yy = q.y * q.y;
    f32 zz = q.z * q.z;
    f32 xy = q.x * q.y;
    f32 xz = q.x * q.z;
    f32 yz = q.y * q.z;
    f32 wx = q.w * q.x;
    f32 wy = q.w * q.y;
    f32 wz = q.w * q.z;

    result.elements[0][0] = 1.0f - 2.0f * (yy + zz);
    result.elements[0][1] = 2.0f * (xy + wz);
    result.elements[0][2] = 2.0f * (xz - wy);

    result.elements[1][0] = 2.0f * (xy - wz);
    result.elements[1][1] = 1.0f - 2.0f * (xx + zz);
    result.elements[1][2] = 2.0f * (yz + wx);

    result.elements[2][0] = 2.0f * (xz + wy);
    result.elements[2][1] = 2.0f * (yz - wx);
    result.elements[2][2] = 1.0f - 2.0f * (xx + yy);

    return result;
}

inline Quaternion mat4_to_quat(Matrix4x4 m) {
    Quaternion result = { 0 };
    f32 four_x_squared_minus_1, four_y_squared_minus_1,
          four_z_squared_minus_1, four_w_squared_minus_1,
          four_biggest_squared_minus_1;
    s32 biggest_index = 0;
    f32 biggest_value, mult;

    four_x_squared_minus_1 = m.elements[0][0] - m.elements[1][1] - m.elements[2][2];
    four_y_squared_minus_1 = m.elements[1][1] - m.elements[0][0] - m.elements[2][2];
    four_z_squared_minus_1 = m.elements[2][2] - m.elements[0][0] - m.elements[1][1];
    four_w_squared_minus_1 = m.elements[0][0] + m.elements[1][1] + m.elements[2][2];

    four_biggest_squared_minus_1 = four_w_squared_minus_1;
    if (four_x_squared_minus_1 > four_biggest_squared_minus_1) {
        four_biggest_squared_minus_1 = four_x_squared_minus_1;
        biggest_index = 1;
    }
    if (four_y_squared_minus_1 > four_biggest_squared_minus_1) {
        four_biggest_squared_minus_1 = four_y_squared_minus_1;
        biggest_index = 2;
    }
    if (four_z_squared_minus_1 > four_biggest_squared_minus_1) {
        four_biggest_squared_minus_1 = four_z_squared_minus_1;
        biggest_index = 3;
    }

    biggest_value = (f32)sqrt(four_biggest_squared_minus_1 + 1.0f) * 0.5f;
    mult = 0.25f / biggest_value;

    switch (biggest_index) {
    case 0:
        result.w = biggest_value;
        result.x = (m.elements[1][2] - m.elements[2][1]) * mult;
        result.y = (m.elements[2][0] - m.elements[0][2]) * mult;
        result.z = (m.elements[0][1] - m.elements[1][0]) * mult;
        break;
    case 1:
        result.w = (m.elements[1][2] - m.elements[2][1]) * mult;
        result.x = biggest_value;
        result.y = (m.elements[0][1] + m.elements[1][0]) * mult;
        result.z = (m.elements[2][0] + m.elements[0][2]) * mult;
        break;
    case 2:
        result.w = (m.elements[2][0] - m.elements[0][2]) * mult;
        result.x = (m.elements[0][1] + m.elements[1][0]) * mult;
        result.y = biggest_value;
        result.z = (m.elements[1][2] + m.elements[2][1]) * mult;
        break;
    case 3:
        result.w = (m.elements[0][1] - m.elements[1][0]) * mult;
        result.x = (m.elements[2][0] + m.elements[0][2]) * mult;
        result.y = (m.elements[1][2] + m.elements[2][1]) * mult;
        result.z = biggest_value;
        break;
    default:
        assert(false);
        break;
    }

    return result;
}

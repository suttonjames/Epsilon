#ifndef MATHS_H
#define MATHS_H

#include <math.h>

#define PI 3.1415926535897f

inline f32 to_radians(f32 degrees) { return degrees * (PI / 180.0f); }

typedef union Vector2 {
    struct { f32 x, y; };
    struct { f32 u, v; };
    struct { f32 width, height; };
    f32 elements[2];
} Vector2;

inline Vector2 vec2(f32 x, f32 y) { return (Vector2){ x, y }; }

inline Vector2 vec2_add(Vector2 a, Vector2 b) { return vec2(a.x + b.x, a.y + b.y); }
inline Vector2 vec2_sub(Vector2 a, Vector2 b) { return vec2(a.x - b.x, a.y - b.y); }
inline Vector2 vec2_mul(Vector2 a, Vector2 b) { return vec2(a.x * b.x, a.y * b.y); }
inline Vector2 vec2_mul_float(Vector2 a, f32 b) { return vec2(a.x * b, a.y * b); }
inline Vector2 vec2_div(Vector2 a, Vector2 b) { return vec2(a.x / b.x, a.y / b.y); }

typedef union Vector3 {
    struct { f32 x, y, z; };
    struct { f32 r, g, b; };
    f32 elements[3];
} Vector3;

inline Vector3 vec3(f32 x, f32 y, f32 z) { return (Vector3){ x, y, z }; }

inline Vector3 vec3_add(Vector3 a, Vector3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline Vector3 vec3_sub(Vector3 a, Vector3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }
inline Vector3 vec3_mul(Vector3 a, Vector3 b) { return vec3(a.x * b.x, a.y * b.y, a.z * b.z); }
inline Vector3 vec3_mul_float(Vector3 a, f32 b) { return vec3(a.x * b, a.y * b, a.z * b); }
inline Vector3 vec3_div(Vector3 a, Vector3 b) { return vec3(a.x / b.x, a.y / b.y, a.z / b.z); }

inline f32 vec3_length_sq(Vector3 v) { return (v.x * v.x + v.y * v.y + v.z * v.z); }
inline f32 vec3_length(Vector3 v) { return (f32)sqrt(vec3_length_sq(v)); }
inline Vector3 vec3_norm(Vector3 v) { return vec3_mul_float(v, (1.0f / vec3_length(v))); }

inline f32 vec3_dot(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline Vector3 vec3_cross(Vector3 a, Vector3 b) {
    return vec3(a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

typedef union Vector4 {
    struct { f32 x, y, z, w; };
    struct { f32 r, g, b, a; };
    f32 elements[4];
} Vector4;

inline Vector4 vec4(f32 x, f32 y, f32 z, f32 w) { return (Vector4){ x, y, z, w }; }

inline Vector4 vec4_add(Vector4 a, Vector4 b) { return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
inline Vector4 vec4_sub(Vector4 a, Vector4 b) { return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
inline Vector4 vec4_mul(Vector4 a, Vector4 b) { return vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
inline Vector4 vec4_div(Vector4 a, Vector4 b) { return vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w); }

typedef union Matrix4x4 {
    f32 elements[4][4];
    f32 item[16];
} Matrix4x4;

inline Matrix4x4 mat4(f32 f);

inline Matrix4x4 mat4_mul(Matrix4x4 a, Matrix4x4 b);
inline Matrix4x4 mat4_mul_float(Matrix4x4 m, f32 f);

inline Matrix4x4 mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near_z, f32 far_z);
inline Matrix4x4 mat4_perspective(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z);

inline Matrix4x4 mat4_translate(Vector3 translation);
inline Matrix4x4 mat4_rotate(f32 angle, Vector3 axis);
inline Matrix4x4 mat4_scale(Vector3 scale);

inline Matrix4x4 mat4_lookat(Vector3 eye, Vector3 centre, Vector3 up);

typedef union Quaternion {
    struct {
        struct { f32 x, y, z; };
        f32 w;
    };
    f32 elements[4];
} Quaternion;

inline Quaternion quat_mul(Quaternion a, Quaternion b);
inline Quaternion quat_mul_float(Quaternion q, f32 f) { return (Quaternion){ q.x * f, q.y * f, q.z * f, q.w * f}; }
inline Vector3 quat_mul_vec3(Quaternion q, Vector3 v);

inline f32 quat_length_sq(Quaternion q) { return (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w); }
inline f32 quat_length(Quaternion q) { return (f32)sqrt(quat_length_sq(q)); }
inline Quaternion quat_norm(Quaternion q) { return quat_mul_float(q, (1.0f / quat_length(q))); }

inline Quaternion quat_conjugate(Quaternion q);

inline Quaternion angle_axis(f32 angle, Vector3 axis);

inline Matrix4x4 quat_to_mat4(Quaternion q);
inline Quaternion mat4_to_quat(Matrix4x4 m);

#endif /* MATHS_H */

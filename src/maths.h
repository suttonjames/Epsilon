#ifndef MATHS_H
#define MATHS_H

#include <math.h>

#define PI 3.1415926535897f

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
inline Vector3 vec3_div(Vector3 a, Vector3 b) { return vec3(a.x / b.x, a.y / b.y, a.z / b.z); }

#endif /* MATHS_H */

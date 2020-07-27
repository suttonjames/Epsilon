#ifndef LANGUAGE_LAYER_H
#define LANGUAGE_LAYER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef size_t usize;

typedef float f32;
typedef double f64;

typedef s32 b32;
#define true 1
#define false 0

#define kilobytes(Value) ((Value)*1024)
#define megabytes(Value) (kilobytes(Value)*1024)
#define gigabytes(Value) (megabytes(Value)*1024)

#endif /* LANGUAGE_LAYER_H */

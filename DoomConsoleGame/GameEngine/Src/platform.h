
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <Windows.h>

typedef char		i8;
typedef short		i16;
typedef int			i32;
typedef long long	i64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef float		r32;
typedef double		r64;

typedef size_t		mem_size;

#ifdef _DEBUG
#define Assert(Exp) { if(!Exp) { *(int *)0 = 0; } }
#elif
#define Assert(Exp)
#endif // DEBUG

#define ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#define Kilobytes(Size) (Size * 1024L)
#define Megabytes(Size) (Kilobytes(Size) * 1024)
#define Gigabytes(Size) (Megabytes(Size) * 1024)
#define Maximum(a, b) (a > b) ? a : b
#define Minimum(a, b) (a < b) ? a : b

#define PI 3.14159265359f

#endif

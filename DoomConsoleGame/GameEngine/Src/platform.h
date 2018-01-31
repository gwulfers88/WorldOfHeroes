
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <Windows.h>

#ifdef _DEBUG
#define Assert(Exp) { if(!Exp) { *(int *)0 = 0; } }
#elif
#define Assert(Exp)
#endif // DEBUG

#define ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#define Kilobytes(Size) (Size * 1024)
#define Megabytes(Size) (Kilobytes(Size) * 1024)
#define Gigabytes(Size) (Megabytes(Size) * 1024)

#define PI 3.14159265359f

#endif


#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <Windows.h>

// Types used
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

// Assertion
#ifdef _DEBUG
#define Assert(Exp) { if(!Exp) { *(int *)0 = 0; } }
#elif
#define Assert(Exp)
#endif // DEBUG

// Helper functions
#define ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#define Kilobytes(Size) (Size * 1024L)
#define Megabytes(Size) (Kilobytes(Size) * 1024)
#define Gigabytes(Size) (Megabytes(Size) * 1024)
#define Maximum(a, b) (a > b) ? a : b
#define Minimum(a, b) (a < b) ? a : b

#define PI 3.14159265359f

#define global_variable static
#define internal_ static

// The frequency in which the CPU is running at.
global_variable u64 GlobalPerforceFreq = 0;

// returns the current time
internal_ u64 win32_GetClockTime()
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return Result.QuadPart;
}

// returns the seconds elapsed based on 2 different time stamps.
internal_ r64 win32_GetSecondsElapsed(u64 Start, u64 End)
{
	r64 Result = ((r64)(End - Start) / (r64)GlobalPerforceFreq);
	return Result;
}

struct game_button
{
	bool pressed;
};

union game_controller
{
	struct 
	{
		game_button MoveUp;
		game_button MoveDown; 
		game_button MoveLeft;
		game_button MoveRight;
		
		game_button ActionUp;
		game_button ActionDown;
		game_button ActionLeft;
		game_button ActionRight;

		game_button Start;
		game_button Back;
	};

	game_button Buttons[10];
};

#endif

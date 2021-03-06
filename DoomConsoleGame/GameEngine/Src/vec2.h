#ifndef VEC2_H
#define VEC2_H

#include "platform.h"
#include <math.h>

/// Scalar Operations

internal_ inline r32
SquareRoot(r32 A)
{
	r32 Result = (r32)sqrt(A);
	return Result;
}

internal_ inline r32
Pow(r32 A, r32 B)
{
	r32 Result = (r32)pow(A, B);
	return Result;
}

internal_ inline i32
RoundReal32ToInt32(r32 Real32)
{
	i32 Result = (i32)roundf(Real32);
	return(Result);
}

internal_ inline u32
RoundReal32ToUInt32(r32 Real32)
{
	u32 Result = (u32)roundf(Real32);
	return(Result);
}

internal_ r32 Square(r32 val)
{
	r32 result = val * val;
	return result;
}

internal_ r32 Lerp(r32 a, r32 t, r32 b)
{
	r32 result = (1.0f - t)*a + t*b;
	return result;
}

internal_ r32 Clamp(r32 min, r32 val, r32 max)
{
	r32 result = val;
	if (val < min)
	{
		result = min;
	}
	else if (val > max)
	{
		result = max;
	}
	return result;
}

internal_ r32 Clamp01(r32 val)
{
	r32 result = Clamp(0.0f, val, 1.0f);
	return result;
}

internal_ r32 Clamp01MapToRange(r32 min, r32 t, r32 max)
{
	r32 result = 0.0f;
	r32 range = max - min;
	if (range != 0.0f)
	{
		result = Clamp01((t - min) / range);
	}
	return result;
}

/// Vector 2 Operations
internal_ union vec2
{
	struct
	{
		r32 x, y;
	};

	r32 E[2];
};

internal_ vec2 Vec2(r32 x, r32 y)
{
	vec2 result = {};
	result.x = x;
	result.y = y;
	return result;
}

internal_ vec2 operator*(r32 a, vec2 b)
{
	vec2 result = {};
	result.x = a * b.x;
	result.y = a * b.y;
	return result;
}

internal_ vec2 operator*(vec2 b, r32 a)
{
	vec2 result = a*b;
	return result;
}

internal_ vec2& operator*=(vec2& b, r32 a)
{
	b = a * b;
	return b;
}

internal_ vec2 operator-(vec2 a)
{
	vec2 result;
	result.x = -a.x;
	result.y = -a.y;
	return result;
}

internal_ vec2 operator-(vec2 a, vec2 b)
{
	vec2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

internal_ vec2 operator-=(vec2& a, vec2 b)
{
	a = a - b;
	return a;
}

internal_ vec2 operator+(vec2 a, vec2 b)
{
	vec2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

internal_ vec2& operator+=(vec2& a, vec2 b)
{
	a = a + b;
	return a;
}

internal_ vec2 Hadamard(vec2 a, vec2 b)
{
	vec2 result = { a.x * b.x, a.y * b.y };
	return result;
}

internal_ r32 Dot(vec2 a, vec2 b)
{
	r32 result = a.x*b.x + a.y*b.y;
	return result;
}

internal_ r32 LengthSq(vec2 a)
{
	r32 result = Dot(a, a);
	return result;
}

internal_ r32 Length(vec2 a)
{
	r32 result = (r32)sqrt(LengthSq(a));
	return result;
}

internal_ vec2 Normalize(vec2 a)
{
	r32 Len = Length(a);
	vec2 Result = {};
	if (Len > 0)
	{
		Result.x = a.x / Len;
		Result.y = a.y / Len;
	}
	return Result;
}

internal_ vec2 Perp(vec2 a)
{
	vec2 result = { -a.y, a.x };
	return result;
}

internal_ vec2 Clamp01(vec2 val)
{
	vec2 result;
	result.x = Clamp01(val.x);
	result.y = Clamp01(val.y);
	return result;
}

// Checks to see if this point is intersecting with the collision box
internal_ bool Intersects(vec2 point, vec2 minP, vec2 maxP)
{
	return (point.x > minP.x &&
		point.x < maxP.x &&
		point.y > minP.y &&
		point.y < maxP.y);
}

internal_ bool operator <(const vec2& lhs, const vec2& rhs)
{
	return (LengthSq(lhs) < LengthSq(rhs));
}

struct rect2
{
	vec2 min;
	vec2 max;
};

internal_ rect2 Rect2(vec2 min, vec2 max)
{
	rect2 result = {};
	result.min = min;
	result.max = max;
	return result;
}

// Checks to see if this point is intersecting with the collision box
internal_ bool Intersects(vec2 point, rect2 rect)
{
	return Intersects(point, rect.min, rect.max);
}

// Grows rectangle by half dimension
internal_ rect2 GrowRect(rect2 rect, vec2 halfDims)
{
	rect.min -= halfDims;
	rect.max += halfDims;
	return rect;
}

#endif
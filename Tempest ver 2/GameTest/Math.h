#pragma once

#include "Vector.h"
#include <math.h>
#include <stdlib.h>

//Custom math class to aid with vector calculations
namespace Math {
	inline float Dot(Vec2 a, Vec2 b) {
		return (a.x * b.x) + (a.y * b.y);
	}

	inline float Dot(Vec3 a, Vec3 b) {
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}
	inline float Mag(Vec2 a) {
		return sqrtf((a.x * a.x) + (a.y * a.y));
	}
	
	inline float MagSq(Vec2 a) {
		return ((a.x * a.x) + (a.y * a.y));
	}

	inline Vec2 Normalize(Vec2 a) {
		return Vec2((a.x / Mag(a)), (a.y / Mag(a)));
	}

	inline Vec3 Cross(Vec3 a, Vec3 b) {
		Vec3 result;
		
		result.x = (a.y * b.z) - (a.z * b.y);
		result.y = (a.z * b.x) - (a.x * b.z);
		result.z = (a.x * b.y) - (a.y * b.x);

		return result;
	}

	inline float AngleDiff(Vec2 a, Vec2 b) {
		float num = Dot(a, b);
		float den = Mag(a) * Mag(b);
		
		return acosf(num / den);
	}
}


#pragma once
#include <iostream>
#include <amp_math.h>

using namespace concurrency::fast_math;

class Vec3 {
public:

	union {
		struct { float x, y, z; };
		float Data[3];
	};

	Vec3() { }
	Vec3() restrict(amp) { }

	Vec3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3(float x, float y, float z) restrict(amp) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3 operator- (Vec3 V) restrict(amp) {
		Vec3 N(x - V.x, y - V.y, z - V.z);
		return N;
	}

	Vec3 operator+ (Vec3 V) restrict(amp) {
		Vec3 N(x + V.x, y + V.y, z + V.z);
		return N;
	}

	Vec3 operator+= (Vec3 V) restrict(amp) {
		x += V.x;
		y += V.y;
		z += V.z;
	}

	Vec3 operator* (int M) restrict(amp) {
		Vec3 N(x * M, y * M, z * M);

		return N;
	}

	Vec3 operator* (float M) restrict(amp) {
		Vec3 N(x * M, y * M, z * M);

		return N;
	}

	Vec3 operator* (Vec3 V) restrict(amp) {
		Vec3 N(V.x * x, V.y * y, V.z * z);
		return N;
	}

	float dot(Vec3 V) restrict(amp) {
		return V.x * x + V.y * y + V.z * z;
	}

	Vec3 cross(Vec3 V) restrict(amp) {
		Vec3 N(
			y * V.z - z * V.y,
			z * V.x - x * V.z,
			x * V.y - y * V.x);
		return N;
	}

	void normalise() restrict(amp) { float n = sqrt(x * x + y * y + z * z); x /= n; y /= n; z /= n; }

	float norm2() restrict(amp) { return dot(*this); }
	float norm() restrict(amp) { return sqrt(norm2()); }
};
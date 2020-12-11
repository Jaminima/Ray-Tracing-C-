#pragma once

#include "vec3.h"
#include <amp.h>
using namespace concurrency::fast_math;

class Camera {
private:
	const float Pi = 3.14f, Pi2 = Pi * 2;
	float cosx, cosy, cosz, sinx, siny, sinz;

public:

	Vec3 Position = Vec3(0, 0, -20);
	Vec3 Angle = Vec3(0, 0, 0);

	void RotateCamera(Vec3 rot) {
		Angle = rot + Angle + Vec3(Pi, Pi, Pi);

		Angle.x = fmodf(Angle.x, Pi2) - Pi;
		Angle.y = fmodf(Angle.y, Pi2) - Pi;
		Angle.z = fmodf(Angle.z, Pi2) - Pi;

		cosx = cosf(Angle.x);
		cosy = cosf(Angle.y);
		cosz = cosf(Angle.z);

		sinx = sinf(Angle.x);
		siny = sinf(Angle.y);
		sinz = sinf(Angle.z);
	}

	Vec3 RotateDirection(Vec3 dir) restrict(amp) {
		dir = Vec3(
			dir.x,
			(cosx * dir.y) + (sinx * dir.z),
			(cosx * dir.z) - (sinx * dir.y));

		dir = Vec3(
			(cosy * dir.x) + (siny * dir.z),
			dir.y,
			(cosy * dir.z) - (siny * dir.x));

		dir = Vec3(
			(cosz * dir.x) + (sinz * dir.y),
			(cosz * dir.y) - (sinz * dir.x),
			dir.z);

		return dir;
	}

	int degreesFOV = 90; //90 Normal, 180 Wide, 180> 180 but W---I---D---E
	float fov = degreesFOV / 90.0f;
};

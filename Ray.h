#pragma once
#include "vec3.h"

class Ray {
public:
	Vec3 Origin = Vec3(0, 0, -20), Direction;

	Ray() {}
	Ray() restrict(amp) {}

	Ray(Vec3 O, Vec3 D) { Origin = O; Direction = D; }
	Ray(Vec3 O, Vec3 D) restrict(amp) { Origin = O; Direction = D; }
};
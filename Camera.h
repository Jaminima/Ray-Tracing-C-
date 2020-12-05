#pragma once

#include "vec3.h"

class Camera {
public:
	Vec3 Position = Vec3(0, 0, -20);
	Vec3 Angle = Vec3(0, 0, 0);

	int degreesFOV = 90; //90 Normal, 180 Wide, 180> 180 but W---I---D---E
	float fov = degreesFOV / 90.0f;
};

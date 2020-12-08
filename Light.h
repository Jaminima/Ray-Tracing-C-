#pragma once
#include "Color.h"
#include "vec3.h"

class Light {
public:
	Vec3 Position = Vec3(0, 0, 0);
	Vec3 colormul = Vec3(1.0f, 0.5f, 1.0f);
	float FadeOff = 4.0f;
};

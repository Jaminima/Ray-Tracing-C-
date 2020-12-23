#pragma once
#include "Color.h"
#include "vec3.h"

class Light
{
public:
	Vec3 Position = Vec3(0, 0, 0);
	Vec3 colormul = Vec3(1.0f, 1.0f, 1.0f);
	float FadeOff = 15.0f;
};

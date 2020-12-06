#pragma once
#include "Color.h"
#include "vec3.h"

class Light {
public:
	Vec3 Position = Vec3(0, 0, 0);
	Color color = Color(255, 255, 255);
	float FadeOff = 0.3f;
};
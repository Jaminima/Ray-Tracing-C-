#pragma once
#include "vec3.h"
#include "Ray.h"
#include "Color.h"

#include "Const.h"

#include <amp.h>
using namespace concurrency;

class SceneObject {
public:
	float reflectivity = 0.9f;
	Color color = Color(255, 255, 255);

	SceneObject() restrict(amp,cpu) {}

	Vec3 ApproxPosition() restrict(amp,cpu) {
		return Vec3(0, 0, 0);
	}

	float RayHitDistance(Ray r) restrict(amp, cpu) { return 0; }

	float CorrectDistance(Ray r, float d) restrict(amp, cpu) { return d; }

	bool RayHit(Ray r) restrict(amp, cpu) { return false; }

	Vec3 IntersectionPoint(Ray r, float Distance) restrict(amp, cpu) {
		return r.Origin + (r.Direction * Distance);
	}

	Ray PointNormal(Vec3 Point, Ray oRay) restrict(amp, cpu) { return oRay; }
};
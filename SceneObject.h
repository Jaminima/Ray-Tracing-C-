#pragma once
#include "vec3.h"
#include "Ray.h"

#include "Const.h"

#include <amp.h>
using namespace concurrency;

struct SceneObject {
public:
	SceneObject(){}
	SceneObject() restrict(amp){}

protected:
	float RayHitDistance(Ray r) restrict(amp);

	bool RayHit(Ray r) restrict(amp);

	Vec3 IntersectionPoint(Ray* Ray, float Distance) restrict(amp);

	Ray Sphere_PointNormal(Vec3 Point, Ray* oRay) restrict(amp);
};
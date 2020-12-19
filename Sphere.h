#pragma once
#include "SceneObject.h"

class Sphere : public SceneObject {
public:
	Sphere() {}
	Sphere() restrict(amp) {}

	Vec3 ApproxPosition() restrict(amp);

	float RayHitDistance(Ray r) restrict(amp);

	bool RayHit(Ray r) restrict(amp);

	Vec3 IntersectionPoint(Ray* Ray, float Distance) restrict(amp);

	Vec3 IntersectionPoint(Ray r, float Distance) restrict(amp) {
		SceneObject::IntersectionPoint(r, Distance);
	}

	Ray PointNormal(Vec3 Point, Ray oRay) restrict(amp);
};

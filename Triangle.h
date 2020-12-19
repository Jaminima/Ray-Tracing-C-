#pragma once
#include "SceneObject.h"

class Triangle : public SceneObject {
public:
	Triangle(){}
	Triangle() restrict(amp) {}

	Vec3 ApproxPosition() restrict(amp);

	float RayHitDistance(Ray r) restrict(amp);

	float CorrectDistance(Ray r, float d) restrict(amp);

	bool RayHit(Ray r) restrict(amp);

	Vec3 IntersectionPoint(Ray r, float Distance) restrict(amp) {
		SceneObject::IntersectionPoint(r, Distance);
	}

	Ray PointNormal(Vec3 Point, Ray oRay) restrict(amp);
};
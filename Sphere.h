#pragma once
#include "Color.h"
#include "vec3.h"
#include "SceneObject.h"

class Sphere : public SceneObject {
public:
	float radius = 3, reflectivity = 0.9f;
	Color color = Color(255, 255, 255);
	Vec3 Center = Vec3(0, 0, 0);

	Sphere() {}
	Sphere() restrict(amp) {}

	float RayHitDistance(Ray r) restrict(amp);

	bool RayHit(Ray r) restrict(amp);

	Vec3 IntersectionPoint(Ray* Ray, float Distance) restrict(amp);

	Ray Sphere_PointNormal(Vec3 Point, Ray* oRay) restrict(amp);
};

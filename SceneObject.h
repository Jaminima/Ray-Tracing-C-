#pragma once
#include "vec3.h"

class Ray {
public:
	Vec3 Origin = Vec3(0,0,-20), Direction;
};

class SceneObject {
public:
	Vec3 Center;
	Vec3 Colour;

	virtual bool Intersects(Ray* Ray) { return 0; }

	virtual float IntersectionDistance(Ray* Ray) { return 0; }

	virtual Ray* PointNormal(Vec3 Point, Ray* oRay) { return 0x0; }

	Vec3 IntersectionPoint(Ray* Ray, float Distance) {
		return Ray->Origin + (Ray->Direction * Distance);
	}
};






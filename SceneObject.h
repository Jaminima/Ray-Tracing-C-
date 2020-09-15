#pragma once
#include "vec3.h"

class Ray {
public:
	Vec3 Origin = Vec3(0, 0, -20), Direction;

	Ray() {}
	Ray() restrict(amp) {}
};

class SceneObject {
public:
	Vec3 Colour;

	SceneObject(Vec3 Col) {
		this->Colour = Col;
	}

	//virtual SceneObject* HitObject(Ray* Ray) { return this; }

	float CorrectDistance(Ray* Ray, float D) restrict(amp) { return D; }

	bool Intersects(Ray* Ray) restrict(amp) { return 0; }

	float IntersectionDistance(Ray* Ray) restrict(amp) { return 0; }

	Ray PointNormal(Vec3 Point, Ray* oRay) restrict(amp) { return Ray(); }

	Vec3 IntersectionPoint(Ray* Ray, float Distance) restrict(amp) {
		return Ray->Origin + (Ray->Direction * Distance);
	}
};

#pragma once
#include "vec3.h"

class Ray {
public:
	Vec3 Origin = Vec3(0,0,-20), Direction;
};

class Colour {
public:
	float R = 0, G = 0, B = 0;
};

class SceneObject {
public:
	Vec3 Center;
	Colour Colour;

	bool Intersects(Ray* Ray);
};

class Sphere : SceneObject {
public:
	float radius = 0;

	bool Intersects(Ray* Ray) {
		Vec3 oc = Ray->Origin - Center;

		float a = Ray->Direction.dot(Ray->Direction),
			b = 2 * oc.dot(Ray->Direction),
			c = oc.dot(oc) - radius * radius;

		return (b * b - 4 * a * c) > 0;
	}

	void SetCenter(Vec3 NewPos) {
		Center = NewPos;
	}
};


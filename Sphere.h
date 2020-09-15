#pragma once
#include "SceneObject.h"
#include <amp_math.h>

class Sphere : public SceneObject {
public:
	Vec3 Center;

	float radius = 0;

	Sphere(Vec3 Cent, Vec3 Col, float r) : SceneObject(Col) {
		this->Center = Cent;

		this->Center.y *= -1;

		this->radius = r;
	}

	bool Intersects(Ray* Ray) restrict(amp) {
		return IntersectionDistance(Ray) >= 0;
	}

	float IntersectionDistance(Ray* Ray) restrict(amp)  {
		Vec3 oc = Ray->Origin - Center;

		float a = Ray->Direction.dot(Ray->Direction),
			b = 2 * oc.dot(Ray->Direction),
			c = oc.dot(oc) - radius * radius;

		float discriminant = (b * b - 4 * a * c);

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			float d = (-b - concurrency::fast_math::sqrtf(discriminant)) / (2.0 * a);
			if (d < 0) return -1.0;
			else return d;
		}
	}

	Ray PointNormal(Vec3 Point, Ray* oRay) restrict(amp) {
		Ray nRay;

		nRay.Direction = (((Point - Center) * oRay->Direction.dot(Point - Center))* 2- oRay->Direction)*-1;
		nRay.Origin = Point;

		return nRay;
	}
};
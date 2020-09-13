#pragma once
#include "SceneObject.h"

class Sphere : public SceneObject {
public:
	float radius = 0;

	Sphere(Vec3 Cent, Vec3 Col, float r) : SceneObject(Cent, Col) {
		this->radius = r;
	}

	bool Intersects(Ray* Ray) override {
		return IntersectionDistance(Ray) >= 0;
	}

	float IntersectionDistance(Ray* Ray) override {
		Vec3 oc = Ray->Origin - Center;

		float a = Ray->Direction.dot(Ray->Direction),
			b = 2 * oc.dot(Ray->Direction),
			c = oc.dot(oc) - radius * radius;

		float discriminant = (b * b - 4 * a * c);

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			float d = (-b - sqrt(discriminant)) / (2.0 * a);
			if (d < 0) return -1.0;
			else return d;
		}
	}

	Ray* PointNormal(Vec3 Point, Ray* oRay) override {
		Ray* nRay = new Ray();

		nRay->Direction = -(2 * (oRay->Direction.dot(Point - Center)) * (Point - Center) - oRay->Direction);
		nRay->Origin = Point;

		return nRay;
	}
};
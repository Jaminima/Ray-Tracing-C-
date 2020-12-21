#pragma once
#include "SceneObject.h"

class Sphere : public SceneObject {
public:
	float radius = 3; //Sphere
	Vec3 Center = Vec3(0, 0, 0);

	Sphere() restrict(amp, cpu) { }

	Vec3 ApproxPosition() restrict(amp, cpu) {
		return Center;
	}

	float RayHitDistance(Ray r) restrict(amp, cpu) {
		Vec3 oc = r.Origin.operator-(Center);

		float a = r.Direction.dot(r.Direction),
			b = 2 * oc.dot(r.Direction),
			c = oc.dot(oc) - (radius * radius);

		float discriminant = (b * b - 4 * a * c);

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			float d = (-b - sqrtf(discriminant)) / (2.0 * a);
			if (d < 0) return -1.0;
			else return d;
		}
	}

	bool RayHit(Ray r) restrict(amp, cpu) {
		return this->RayHitDistance(r) >= 0;
	}

	Vec3 IntersectionPoint(Ray Ray, float Distance) restrict(amp, cpu) {
		return Ray.Origin + (Ray.Direction * Distance);
	}

	Ray PointNormal(Vec3 Point, Ray oRay) restrict(amp, cpu) {
		Ray nRay;

		nRay.Direction = (((Point - Center) * oRay.Direction.dot(Point - Center)) * 2 - oRay.Direction) * -1;
		nRay.Origin = Point;

		return nRay;
	}
};

#include "Sphere.h"

float Sphere::RayHitDistance(Ray r) restrict(amp) {
	Vec3 oc = r.Origin.operator-(Center);

	float a = r.Direction.dot(r.Direction),
		b = 2 * oc.dot(r.Direction),
		c = oc.dot(oc) - (radius * radius);

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

bool Sphere::RayHit(Ray r) restrict(amp) {
	return this->RayHitDistance(r) >= 0;
}

Vec3 Sphere::IntersectionPoint(Ray* Ray, float Distance) restrict(amp) {
	return Ray->Origin + (Ray->Direction * Distance);
}

Ray Sphere::Sphere_PointNormal(Vec3 Point, Ray* oRay) restrict(amp) {
	Ray nRay;

	nRay.Direction = (((Point - Center) * oRay->Direction.dot(Point - Center)) * 2 - oRay->Direction) * -1;
	nRay.Origin = Point;

	return nRay;
}
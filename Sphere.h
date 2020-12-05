#pragma once
#include "Color.h"
#include "vec3.h"
#include "Ray.h"

#include "Const.h"

#include <amp.h>
using namespace concurrency;

class Sphere {
public:
	float radius = 3;
	Color color = Color(255, 0, 255);
	Vec3 Center = Vec3(5, 0, 0);

	Sphere() {}
	Sphere() restrict(amp) {}

	float RayHitDistance(Ray r, Sphere s) restrict(amp) {
		Vec3 oc = r.Origin.operator-(s.Center);

		float a = r.Direction.dot(r.Direction),
			b = 2 * oc.dot(r.Direction),
			c = oc.dot(oc) - (s.radius * s.radius);

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

	bool RayHit(Ray r, Sphere s) restrict(amp) {
		return RayHitDistance(r, s) >= 0;
	}

	Color RenderRay(index<2> idx) restrict(amp) {
		float vx = (idx[1] / (float)px_half) - 1,
			vy = (idx[0] / (float)px_half) - 1;

		if (Sphere().RayHit(Ray(Vec3(0, 0, -10), Vec3(vx, vy, 1)), Sphere()))
			return Color(255, 255, 255);
		else return Color(0, 0, 0);
	}
};


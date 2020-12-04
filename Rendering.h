#pragma once
#include "SceneObject.h"

#include "List.h"

#include "vec3.h"

#include <amp.h>
using namespace concurrency;

const unsigned int px = 500, py = 500;
const unsigned int px_half = px / 2, py_half = px / 2;

struct Color {
public:
	unsigned int rgba = 0x0;

	void SetR(unsigned int r) restrict(amp) { rgba &= 0x00FFFFFF; rgba |= (r & 0xFF) << 24; }
	void SetG(unsigned int g) restrict(amp) { rgba &= 0xFF00FFFF; rgba |= (g & 0xFF) << 16; }
	void SetB(unsigned int b) restrict(amp) { rgba &= 0xFFFF00FF; rgba |= (b & 0xFF) << 8; }

	Color(unsigned int r, unsigned int g, unsigned int b) {
	}

	Color(unsigned int r, unsigned int g, unsigned int b) restrict(amp) {
		SetR(r);
		SetR(g);
		SetR(b);
	}
};

class Sphere {
public:
	float radius = 1;
	Color color = Color(255,255,255);
	Vec3 Center = Vec3(0,0,0);

	Sphere(){}
	Sphere() restrict(amp) {}
};

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

	if (RayHit(Ray(Vec3(0, 0, -10), Vec3(vx, vx, 1)), Sphere()))
		return Color(255, 255, 255);
	else return Color(0, 0, 0);
}

Color* RenderScene() {
	Color* rgb = (Color*)malloc(px*py*sizeof(Color));

	array_view<Color, 2> ColorView(px, py, rgb);

	parallel_for_each(
		ColorView.extent,
		[=](index<2> idx) restrict(amp) {
			ColorView[idx] = RenderRay(idx);
		}
	);

	ColorView.synchronize();

	return rgb;
}
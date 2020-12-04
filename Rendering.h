#pragma once
#include "SceneObject.h"

#include "List.h"

#include "vec3.h"

#include <amp.h>
using namespace concurrency;

const unsigned int px = 1000, py = 1000;
const unsigned int px_half = px / 2, py_half = px / 2;

struct Color {
public:
	unsigned int rgba = 0xFF000000;

	void SetR(unsigned int r) restrict(amp) { rgba = rgba | ((r & 0xFF) << 16); }
	void SetG(unsigned int g) restrict(amp) { rgba = rgba | ((g & 0xFF) << 8); }
	void SetB(unsigned int b) restrict(amp) { rgba = rgba | ((b & 0xFF)); }

	Color(unsigned int r, unsigned int g, unsigned int b) {
		rgba |= ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));
	}

	Color(unsigned int r, unsigned int g, unsigned int b) restrict(amp) {
		rgba |= ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));
	}
};

class Sphere {
public:
	float radius = 4;
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

	if (RayHit(Ray(Vec3(0, 0, -10), Vec3(vx, vy, 1)), Sphere()))
		return Color(255, 255, 255);
	else return Color(0, 0, 0);
}

Color* RenderScene() {
	Color* rgb = (Color*)malloc(px*py*sizeof(Color));

	array_view<Color, 2> ColorView(px, py, rgb);

	parallel_for_each(
		ColorView.extent,
		[=](index<2> idx) restrict(amp) {
			ColorView[idx[0]][idx[1]] = RenderRay(idx);
		}
	);

	ColorView.synchronize();

	return rgb;
}
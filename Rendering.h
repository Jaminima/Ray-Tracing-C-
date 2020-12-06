#pragma once
#include "Light.h"
#include "Sphere.h"
#include "Const.h"
#include <amp.h>
using namespace concurrency;

#include "GL/glut.h"
#include "GL/freeglut.h"

Color RenderRay(Ray r, array_view<Sphere, 1> spheres, array_view<Light,1> lights) restrict(amp) {
	Color c(0, 0, 0);
	float LastHit;
	Ray hitRay;
	int hitSphere = 0;
	unsigned int reflection = 1;

	while (hitSphere != -1) {
		hitSphere = -1;
		LastHit = 0xFFFFFFFF;

		for (unsigned int i = 0; i < spheres.extent.size(); i++) {
			float RayHit = spheres[i].RayHitDistance(r);

			if (RayHit > 0 && RayHit < LastHit) {
				hitSphere = i;
				hitRay = r;
				LastHit = RayHit;
			}
		}

		if (hitSphere != -1) {
			Vec3 impact = spheres[hitSphere].IntersectionPoint(&hitRay, LastHit);

			Vec3 delta = lights[0].Position - impact;
			float distance = fast_math::sqrtf(delta.dot(delta));

			float lightmul = (1 / (distance * lights[0].FadeOff));
			if (lightmul > 1) lightmul = 1;

			c = c + (spheres[hitSphere].color * (1.0f / reflection) * lightmul);
			r = spheres[hitSphere].Sphere_PointNormal(impact, &hitRay);
		}

		reflection++;
	}

	return c;
}

Color RenderPixel(index<2> idx, array_view<Sphere, 1> spheres, array_view<Light, 1> lights, Camera cam) restrict(amp) {
	float vx = (idx[1] / (float)px_half) - 1,
		vy = (idx[0] / (float)py_half) - 1;

	vx *= cam.fov;
	vy *= cam.fov * (py/(float)px);

	Vec3 Direction = Vec3(vx, vy, 1);

	Direction += cam.Angle;
	Direction.normalise();

	Ray r(cam.Position, Direction);
	return RenderRay(r, spheres, lights);
}

Sphere* spheres;
Light* lights;

Color* RenderScene(Color* rgb) {
	array_view<Color, 2> ColorView(py, px, rgb);
	array_view<Sphere, 1> SphereView(3, spheres);
	array_view<Light, 1> LightView(1, lights);

	Camera cam = mainCamera;

	parallel_for_each(
		ColorView.extent,
		[=](index<2> idx) restrict(amp) {
			ColorView[idx] = RenderPixel(idx, SphereView, LightView, cam);
		}
	);

	ColorView.synchronize();

	return rgb;
}

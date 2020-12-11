#pragma once
#include "Light.h"
#include "Sphere.h"
#include "Const.h"
#include <amp.h>
using namespace concurrency;

#include "GL/glut.h"
#include "GL/freeglut.h"

Vec3 LightMul(Vec3 point, array_view<Sphere, 1> spheres, array_view<Light, 1> lights) restrict(amp) {
	Vec3 delta;
	Vec3 lightmul(0, 0, 0);

	float distance;
	unsigned int lightHits = 0;

	for (unsigned int i = 0; i < lights.extent.size() && lightHits<lightHitLimit; i++) {
		delta = point - lights[i].Position;

		bool hitSomthing = false;

		Ray r(lights[i].Position, delta * -1);

		for (unsigned int j = 0; j < spheres.extent.size(); j++) {
			if (spheres[j].RayHit(r)) {
				hitSomthing = true;
				lightHits++;
				break;
			}
		}

		if (!hitSomthing) {
			distance = fast_math::sqrtf(delta.dot(delta));

			lightmul = (lightmul + (lights[i].colormul * (1.0f - (distance / lights[i].FadeOff))));
		}

		//if (lightmul > 1) lightmul = 1;
	}

	//lightmul = lightmul * (1.0f / lights.extent.size());

	if (lightmul.x > 1) lightmul.x = 1;
	if (lightmul.y > 1) lightmul.y = 1;
	if (lightmul.z > 1) lightmul.z = 1;

	return lightmul;
}

Color RenderRay(Ray r, array_view<Sphere, 1> spheres, array_view<Light, 1> lights) restrict(amp) {
	Color c(0, 0, 0);
	float LastHit, totalReflectivity = 0.0f, reflec = 0.0f;
	Ray hitRay;
	int hitSphere = 0;
	unsigned int reflection = 0;

	while (hitSphere != -1 && reflection<reflectionLimit) {
		hitSphere = -1;
		LastHit = 0xFFFFFFFF;

		for (unsigned int i = 0; i < spheres.extent.size(); i++) {
			float RayHit = spheres[i].RayHitDistance(r);

			if (RayHit > 0) {
				hitSphere = i;
				hitRay = r;
				LastHit = RayHit;
				break;
			}
		}

		if (hitSphere != -1) {
			Vec3 impact = spheres[hitSphere].IntersectionPoint(&hitRay, LastHit);

			if (totalReflectivity > 0.0f) reflec = totalReflectivity;

			c = c + (spheres[hitSphere].color * LightMul(impact, spheres, lights) * (1-reflec));
			r = spheres[hitSphere].Sphere_PointNormal(impact, &hitRay);

			totalReflectivity += sqrtf(spheres[hitSphere].reflectivity);
		}

		reflection++;
	}

	return c;
}

Color RenderPixel(index<2> idx, array_view<Sphere, 1> spheres, array_view<Light, 1> lights, Camera cam) restrict(amp) {
	float vx = (idx[1] / (float)px_half) - 1,
		vy = (idx[0] / (float)py_half) - 1;

	vx *= cam.fov;
	vy *= cam.fov * (py / (float)px);

	Vec3 Direction = Vec3(vx, vy, 1);

	Direction = cam.RotateDirection(Direction);

	/*Direction += cam.Angle;
	Direction.normalise();*/

	Ray r(cam.Position, Direction);
	return RenderRay(r, spheres, lights);
}

Sphere* spheres = new Sphere[totalSpheres];
Light* lights = new Light[totalLights];

array_view<Sphere, 1> SphereView(totalSpheres, spheres);


void OrderCamera() {
	auto dupe = SphereView;
	array_view<Sphere, 1> FinalSphereView(totalSpheres, spheres);
	array_view<float, 1> DistanceView(totalSpheres);

	Camera cam = mainCamera;

	parallel_for_each(
		SphereView.extent,
		[=](index<1> idx) restrict(amp) {
			Vec3 x = dupe[idx].Center - cam.Position;
			DistanceView[idx] = sqrtf(x.dot(x));
		}
	);

	parallel_for_each(
		dupe.extent,
		[=](index<1> idx) restrict(amp) {
			
	}
	);
}


void RenderScene(array_view<Color,2> rgb) {
	array_view<Light, 1> LightView(totalLights, lights);
	array_view<Sphere, 1> SphereView(totalSpheres, spheres);

	Camera cam = mainCamera;

	parallel_for_each(
		rgb.extent,
		[=](index<2> idx) restrict(amp) {
			rgb[idx] = RenderPixel(idx, SphereView, LightView, cam);
		}
	);
}

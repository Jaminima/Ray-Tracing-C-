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

			lightmul = (lightmul + (lights[i].colormul * (lights[i].FadeOff / distance)));
		}

		//if (lightmul > 1) lightmul = 1;
	}

	lightmul = lightmul * (1.0f / lights.extent.size());

	if (lightmul.x > 1) lightmul.x = 1;
	if (lightmul.y > 1) lightmul.y = 1;
	if (lightmul.z > 1) lightmul.z = 1;

	return lightmul;
}

Color RenderRay(Ray r, array_view<Sphere, 1> spheres, array_view<Light, 1> lights) restrict(amp) {
	Color c(0, 0, 0);
	float LastHit;
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

			c = c + (spheres[hitSphere].color * LightMul(impact, spheres, lights) * (1.0f / (reflection+1)));
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
	vy *= cam.fov * (py / (float)px);

	Vec3 Direction = Vec3(vx, vy, 1);

	Direction = cam.RotateDirection(Direction);

	/*Direction += cam.Angle;
	Direction.normalise();*/

	Ray r(cam.Position, Direction);
	return RenderRay(r, spheres, lights);
}

Sphere* spheres;
Light* lights;


void OrderCamera() {
	array_view<Sphere, 1> SphereView(totalSpheres, spheres);
	array_view<Sphere, 1> FinalSphereView(totalSpheres, spheres);
	array_view<float, 1> DistanceView(totalSpheres);
	//array_view<unsigned int, 1> SphereOrder(totalSpheres);

	Camera cam = mainCamera;

	parallel_for_each(
		SphereView.extent,
		[=](index<1> idx) restrict(amp) {
		DistanceView[idx] = sqrtf(SphereView[idx].Center.dot(cam.Position));
		}
	);

	parallel_for_each(
		SphereView.extent,
		[=](index<1> idx) restrict(amp) {
		float nthValue = DistanceView[0];
		unsigned int nth = 0;
		unsigned int lessthan=0;

		for (unsigned int i = 0;i < DistanceView.extent[0];i++) {
			if (nthValue > DistanceView[i]) { 
				if (lessthan == idx[0]) {
					nth = idx[0];
					break;
				}
				else {
					lessthan++;
					nthValue = DistanceView[i]; 
					nth = i;
				}
			}
		}

		//SphereOrder[idx] = nth;
		FinalSphereView[nth] = SphereView[idx];
	}
	);

	FinalSphereView.synchronize_async();
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

#pragma once
#include "Light.h"
#include "SceneObjectManager.h"
#include "Const.h"
#include <amp.h>
//using namespace concurrency;
using namespace concurrency::fast_math;

struct Hit {
public:
	bool hasHit = false;
	float distance = 0;
	unsigned int objectIndex = 0;

	Hit() restrict(amp, cpu) {}
};

bool HitsObject(Ray r, float distLimit, array_view<SceneObjectManager, 1> SceneObjects) restrict(amp, cpu) {
	for (unsigned int i = 0;i < SceneObjects.extent.size();i++) {
		float dist = SceneObjects[i].RayHitDistance(r);

		if (dist > 0 && dist < distLimit) return true;
	}
	return false;
}

Vec3 LightMul(Vec3 point, Camera cam, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights) restrict(amp, cpu) {
	Vec3 lightmul(0, 0, 0);

	Vec3 delta;
	Ray r;

	unsigned int lightHits = 0;

	for (unsigned int i = 0;i < lights.extent.size();i++) {
		delta = point - lights[i].Position;

		float dist = sqrtf(delta.dot(delta));

		r = Ray(lights[0].Position, delta * -1);

		if (!HitsObject(r, dist, SceneObjects)) { 
			lightmul += lights[i].colormul * (1.0f-(dist / lights[i].FadeOff)); 
			lightHits++;
		}
	}

	//lightmul = lightmul * (1.0f / lightHits);

	lightmul.x = lightmul.x > 1 ? 1 : lightmul.x;
	lightmul.y = lightmul.y > 1 ? 1 : lightmul.y;
	lightmul.z = lightmul.z > 1 ? 1 : lightmul.z;

	return lightmul;
}

Hit ClosestHit(Ray r, array_view<SceneObjectManager, 1> SceneObjects) restrict(amp, cpu) {
	Hit closest = Hit();

	for (unsigned int i = 0;i < SceneObjects.extent.size();i++) {
		float dist = SceneObjects[i].RayHitDistance(r);

		if (dist > 0) {
			dist = SceneObjects[i].CorrectDistance(r, dist);

			if (dist < closest.distance || !closest.hasHit)
			{
				closest.distance = dist; closest.objectIndex = i; closest.hasHit = true;
			}
		}
	}

	return closest;
}

Color RenderRay(Ray r, Camera cam, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights) restrict(amp, cpu) {
	Color c(0, 0, 0);
	Hit closest = Hit();

	bool isFirst = true;
	float reflections = 0;

	Vec3 intersect;

	while ((closest.hasHit || isFirst) && reflections < reflectionLimit) {
		isFirst = false;
		closest = ClosestHit(r, SceneObjects);

		if (!closest.hasHit) break;

		intersect = SceneObjects[closest.objectIndex].IntersectionPoint(r, closest.distance);
		r = SceneObjects[closest.objectIndex].PointNormal(intersect, r);

		reflections++;

		c = c + (SceneObjects[closest.objectIndex].color() * LightMul(intersect, cam, SceneObjects, lights) * (1.0f/reflections));
	}

	return c;
}
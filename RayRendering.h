#pragma once
#include "HitChecks.h"
using namespace concurrency::fast_math;

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
			lightmul += lights[i].colormul * (1.0f - (dist / lights[i].FadeOff));
			lightHits++;
		}
	}

	//lightmul = lightmul * (1.0f / lightHits);

	lightmul.x = lightmul.x > 1 ? 1 : lightmul.x;
	lightmul.y = lightmul.y > 1 ? 1 : lightmul.y;
	lightmul.z = lightmul.z > 1 ? 1 : lightmul.z;

	return lightmul;
}

Color RenderRayReflections(Ray r, Camera cam, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights, int ignoreObject) restrict(amp, cpu) {
	Color c(0, 0, 0);
	Hit closest = Hit();

	bool isFirst = true;
	unsigned int reflections = 1;

	SceneObjectManager curObj;

	while ((closest.hasHit || isFirst) && reflections < reflectionLimit) {
		closest = ClosestHit(r, SceneObjects, ignoreObject);

		curObj = SceneObjects[closest.objectIndex];

		if (!closest.hasHit) break;

		r = curObj.PointNormal(closest.intersect, r);

		reflections++;
		isFirst = false;

		c = c + (curObj.color() * LightMul(closest.intersect, cam, SceneObjects, lights) * (curObj.reflectivity() / reflections));
	}

	return c;
}

Color RenderRay(Ray r, Camera cam, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights) restrict(amp, cpu) {
	Color c(0, 0, 0);
	Hit closest = ClosestHit(r, SceneObjects);

	SceneObjectManager firstObj;

	if (closest.hasHit) {
		firstObj = SceneObjects[closest.objectIndex];

		c = c + (firstObj.color() * LightMul(closest.intersect, cam, SceneObjects, lights));

		c = c + RenderRayReflections(
			firstObj.PointNormal(closest.intersect, r),
			cam, SceneObjects, lights, closest.objectIndex);
	}

	return c * firstObj.opacity();
}
#pragma once
#include "HitChecks.h"
using namespace fast_math;

Vec3 LightMul(Vec3 point, Camera cam, Vec3 WorldLight, array_view<SceneObjectManager, 1> SceneObjects,
	array_view<Light, 1> lights) restrict(amp, cpu)
{
	Vec3 lightmul = Vec3(WorldLight);

	Vec3 delta;
	Ray r;

	unsigned int lightHits = 0;

	for (unsigned int i = 0; i < lights.extent.size(); i++)
	{
		delta = lights[i].Position - point;

		float dist = sqrtf(delta.dot(delta));

		r = Ray(point + (delta * 0.01f), delta * -1);

		if (!HitsObject(r, dist, SceneObjects))
		{
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

Color RenderRayReflections(Ray r, Camera cam, Vec3 WorldLight, array_view<SceneObjectManager, 1> SceneObjects,
	array_view<Light, 1> lights, int ignoreObject) restrict(amp, cpu)
{
	Color c(0, 0, 0);
	Hit closest = Hit();

	bool isFirst = true;
	unsigned int reflections = 1;

	SceneObjectManager curObj;

	while ((closest.hasHit || isFirst) && reflections < reflectionLimit)
	{
		closest = ClosestHit(r, SceneObjects, ignoreObject);

		curObj = SceneObjects[closest.objectIndex];

		if (!closest.hasHit) break;

		r = curObj.PointNormal(closest.intersect, r);

		reflections++;
		isFirst = false;

		c = c + (curObj.color() * LightMul(closest.intersect, cam, WorldLight, SceneObjects, lights) * (curObj.
			reflectivity() / reflections));
	}

	return c;
}

Color RenderRay(Ray r, Camera cam, Vec3 WorldLight, array_view<SceneObjectManager, 1> SceneObjects,
	array_view<Light, 1> lights) restrict(amp, cpu)
{
	Color c(0, 0, 0);
	Hit closest = ClosestHit(r, SceneObjects);

	SceneObjectManager firstObj;

	if (closest.hasHit)
	{
		firstObj = SceneObjects[closest.objectIndex];

		c = c + (firstObj.color() * LightMul(closest.intersect, cam, WorldLight, SceneObjects, lights));

		c = c + RenderRayReflections(
			firstObj.PointNormal(closest.intersect, r),
			cam, WorldLight, SceneObjects, lights, closest.objectIndex);
	}

	if (firstObj.opacity() != 1)
	{
		c = c * firstObj.opacity();
		c = c + (RenderRayReflections(Ray(closest.intersect, r.Direction), cam, WorldLight, SceneObjects, lights,
			closest.objectIndex) * (1 - firstObj.opacity()));
	}
	return c;
}

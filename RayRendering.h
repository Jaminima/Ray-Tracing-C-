#pragma once
#include "HitChecks.h"
using namespace fast_math;

Vec3 LightMul(Vec3 point, Camera cam, Vec3 WorldLight, array_view<Mesh, 1> SceneObjects, array_view<Triangle, 1> SceneTrianglesView,
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

		if (!HitsObject(r, dist, SceneObjects, SceneTrianglesView))
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

Color RenderRayReflections(Ray r, Camera cam, Vec3 WorldLight, Color WorldColor, array_view<Mesh, 1> SceneObjects, array_view<Triangle, 1> SceneTrianglesView,
	array_view<Light, 1> lights, int ignoreObject) restrict(amp, cpu)
{
	Color c(0, 0, 0);

	Hit closest = Hit();

	bool isFirst = true;
	unsigned int reflections = 1;

	Triangle curObj;

	while ((closest.hasHit || isFirst) && reflections < reflectionLimit)
	{
		closest = ClosestHit(r, SceneObjects, SceneTrianglesView, ignoreObject);

		curObj = SceneTrianglesView[closest.triangleIndex];

		if (!closest.hasHit) break;

		r = curObj.PointNormal(closest.intersect, r);

		reflections++;
		isFirst = false;

		c = c + (curObj.color * LightMul(closest.intersect, cam, WorldLight, SceneObjects, SceneTrianglesView, lights) * (curObj.
			reflectivity / reflections));
	}

	return c;
}

Color RenderRay(Ray r, Camera cam, Vec3 WorldLight, Color WorldColor, array_view<Mesh, 1> SceneObjects, array_view<Triangle, 1> SceneTrianglesView,
	array_view<Light, 1> lights) restrict(amp, cpu)
{
	Color c(0, 0, 0);
	Hit closest = ClosestHit(r, SceneObjects, SceneTrianglesView);

	Triangle firstObj;

	if (closest.hasHit)
	{
		firstObj = SceneTrianglesView[closest.triangleIndex];

		c = c + (firstObj.color * LightMul(closest.intersect, cam, WorldLight, SceneObjects, SceneTrianglesView, lights));

		c = c + RenderRayReflections(
			firstObj.PointNormal(closest.intersect, r),
			cam, WorldLight, WorldColor, SceneObjects, SceneTrianglesView, lights, closest.objectIndex);

		if (firstObj.opacity != 1)
		{
			c = c/* * firstObj.opacity()*/;
			c = c + (RenderRayReflections(Ray(closest.intersect, r.Direction), cam, WorldLight, WorldColor, SceneObjects, SceneTrianglesView, lights,
				closest.objectIndex) * (1 - firstObj.opacity));
		}
		return c;
	}
	else return WorldColor;
}

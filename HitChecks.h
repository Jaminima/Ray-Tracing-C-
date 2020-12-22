#pragma once
#include "SceneObjectManager.h"
#include "Light.h"
#include "Const.h"
#include <amp.h>
using namespace concurrency::fast_math;

struct Hit {
public:
	bool hasHit = false;
	float distance = 0;
	unsigned int objectIndex = 0;
	Vec3 intersect;

	Hit() restrict(amp, cpu) {}
};

bool HitsObject(Ray r, float distLimit, array_view<SceneObjectManager, 1> SceneObjects) restrict(amp, cpu) {
	for (unsigned int i = 0;i < SceneObjects.extent.size();i++) {
		float dist = SceneObjects[i].RayHitDistance(r);

		if (dist > 0 && dist < distLimit) return true;
	}
	return false;
}

Hit ClosestHit(Ray r, array_view<SceneObjectManager, 1> SceneObjects, int ignoreObject = -1) restrict(amp, cpu) {
	Hit closest = Hit();

	for (unsigned int i = 0;i < SceneObjects.extent.size();i++) {
		if (i != ignoreObject) {
			float dist = SceneObjects[i].RayHitDistance(r);

			if (dist > 0) {
				dist = SceneObjects[i].CorrectDistance(r, dist);

				if (dist < closest.distance || !closest.hasHit)
				{
					closest.distance = dist;
					closest.objectIndex = i;
					closest.hasHit = true;
					closest.intersect = SceneObjects[i].IntersectionPoint(r, closest.distance);
				}
			}
		}
	}

	return closest;
}
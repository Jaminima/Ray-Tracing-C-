#pragma once
#include "Mesh.h"
#include "Light.h"
#include "Const.h"
#include <amp.h>
using namespace fast_math;

struct Hit
{
public:
	bool hasHit = false;
	float distance = 0;
	unsigned int objectIndex = 0, triangleIndex = 0;
	Vec3 intersect;

	Hit() restrict(amp, cpu)
	{
	}
};

bool HitsObject(Ray r, float distLimit, array_view<Mesh, 1> SceneObjects, array_view<Triangle, 1> SceneTrianglesView) restrict(amp, cpu)
{
	for (unsigned int i = 0; i < SceneObjects.extent.size(); i++)
	{
		Mesh::MeshHit dist = SceneObjects[i].RayHitDistance(r, SceneTrianglesView);

		if (dist.dist != -1)
		{
			dist.dist = SceneObjects[i].CorrectDistance(r, dist.dist);

			if (dist.dist <= distLimit) return false;
		}
	}
	return true;
}

Hit ClosestHit(Ray r, array_view<Mesh, 1> SceneObjects, array_view<Triangle, 1> SceneTrianglesView, int ignoreObject = -1) restrict(amp, cpu)
{
	Hit closest = Hit();

	for (unsigned int i = 0; i < SceneObjects.extent.size(); i++)
	{
		if (i != ignoreObject)
		{
			Mesh::MeshHit dist = SceneObjects[i].RayHitDistance(r, SceneTrianglesView);

			if (dist.dist != -1)
			{
				dist.dist = SceneTrianglesView[closest.triangleIndex].CorrectDistance(r, dist.dist);

				if (dist.dist < closest.distance || !closest.hasHit)
				{
					closest.distance = dist.dist;
					closest.objectIndex = i;
					closest.hasHit = true;
					closest.triangleIndex = dist.triIDX;
					closest.intersect = SceneTrianglesView[closest.triangleIndex].IntersectionPoint(r, closest.distance);
				}
			}
		}
	}

	return closest;
}

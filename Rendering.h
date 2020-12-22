#pragma once
#include "Light.h"
#include "SceneObjectManager.h"
#include "Const.h"
#include "QuickSort.h"
#include <amp.h>
using namespace concurrency;

#include "GL/glut.h"
#include "GL/freeglut.h"

SceneObjectManager* sceneObjects = new SceneObjectManager[totalSceneObjects];
Light* lights = new Light[totalLights];
float* distances = new float[totalSceneObjects];

Vec3 LightMul(Vec3 point, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights) restrict(amp, cpu) {
	Vec3 lightmul(0, 0, 0);

	return lightmul;
}

struct Hit{
public:
	bool hasHit = false;
	float distance = 0;
	unsigned int objectIndex = 0;

	Hit() restrict(amp,cpu){}
};

Hit ClosestHit(Ray r, array_view<SceneObjectManager, 1> SceneObjects) restrict(amp,cpu) {
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

Color RenderRay(Ray r, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights) restrict(amp, cpu) {
	Color c(0, 0, 0);

	Hit closest = ClosestHit(r, SceneObjects);

	if (closest.hasHit) c = SceneObjects[closest.objectIndex].color();

	return c;
}

Color RenderPixel(index<2> idx, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights, Camera cam) restrict(amp, cpu) {
	float vx = (idx[1] / (float)px_half) - 1,
		vy = (idx[0] / (float)py_half) - 1;

	vx *= cam.fov;
	vy *= cam.fov * (py / (float)px);

	Vec3 Direction = Vec3(vx, vy, 1);

	Direction = cam.RotateDirection(Direction);

	/*Direction += cam.Angle;
	Direction.normalise();*/

	Ray r(cam.Position, Direction);
	return RenderRay(r, SceneObjects, lights);
}

//void SortSpheres() {
//	QuickSort(0, totalSceneObjects - 1, distances, sceneObjects);
//}
//
//void OrderCamera() {
//	array_view<SceneObjectManager, 1> SceneView(totalSceneObjects, sceneObjects);
//	array_view<float, 1> DistanceView(totalSceneObjects, distances);
//
//	Camera cam = mainCamera;
//
//	parallel_for_each(
//		SceneView.extent,
//		[=](index<1> idx) restrict(amp) {
//		Vec3 x = SceneView[idx].ApproxPosition() - cam.Position;
//		DistanceView[idx] = sqrtf(x.dot(x));
//	}
//	);
//
//	DistanceView.synchronize_async().then(SortSpheres);
//
//	SceneView.refresh();
//}

void RenderScene(array_view<Color, 2> rgb) {
	array_view<Light, 1> LightView(totalLights, lights);
	array_view<SceneObjectManager, 1> SceneView(totalSceneObjects, sceneObjects);

	Camera cam = mainCamera;

	if (runOnCpu) {
		for (unsigned int x = 0, y = 0;y < py;) {
			rgb[y][x] = RenderPixel(index<2>(y, x), SceneView, LightView, cam);
			x++;
			if (x == px) { x = 0; y++; }
		}
	}
	else {
		parallel_for_each(
			rgb.extent,
			[=](index<2> idx) restrict(amp) {
			rgb[idx] = RenderPixel(idx, SceneView, LightView, cam);
		}
		);
	}
}

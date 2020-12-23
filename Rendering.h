#pragma once
#include "RayRendering.h"

using namespace fast_math;

Camera mainCamera = Camera();

SceneObjectManager* sceneObjects = new SceneObjectManager[totalSceneObjects];
Light* lights = new Light[totalLights];
float* distances = new float[totalSceneObjects];

Color RenderPixel(index<2> idx, array_view<SceneObjectManager, 1> SceneObjects, array_view<Light, 1> lights, Camera cam,
	Vec3 WorldLight) restrict(amp, cpu)
{
	float vx = (idx[1] / static_cast<float>(px_half)) - 1,
		vy = (idx[0] / static_cast<float>(py_half)) - 1;

	vx *= cam.fov;
	vy *= cam.fov * (py / static_cast<float>(px));

	Vec3 Direction = Vec3(vx, vy, 1);

	Direction = cam.RotateDirection(Direction);

	Ray r(cam.Position, Direction);
	return RenderRay(r, cam, WorldLight, SceneObjects, lights);
}

void RenderScene(array_view<Color, 2> rgb)
{
	array_view<Light, 1> LightView(totalLights, lights);
	array_view<SceneObjectManager, 1> SceneView(totalSceneObjects, sceneObjects);

	Camera cam = mainCamera;
	Vec3 wrldLight = WorldLight;

	if (runOnCpu)
	{
		for (unsigned int x = 0, y = 0; y < py;)
		{
			rgb[y][x] = RenderPixel(index<2>(y, x), SceneView, LightView, cam, wrldLight);
			x++;
			if (x == px)
			{
				x = 0;
				y++;
			}
		}
	}
	parallel_for_each(
		rgb.extent,
		[=](index<2> idx) restrict(amp)
	{
		rgb[idx] = RenderPixel(idx, SceneView, LightView, cam, wrldLight);
	}
	);
}

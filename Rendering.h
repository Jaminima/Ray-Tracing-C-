#pragma once
#include "SceneObject.h"

#include "Sphere.h"
#include "Triangle.h"
#include "Mesh.h"

#include "List.h"

#include <thread>

#include "vec3.h"

#include <amp.h>
using namespace concurrency;

const int ViewWidth = 10;
const float ViewSteps = 0.1f;
const int ViewWS = 200;

//12 * pow(ViewWidth / ViewSteps, 2)
const int imageMemory = 120000;

Vec3 CalculateRayColour(Ray R, array_view<SceneObject> Objs, int ObjsSize) restrict(amp) {
	Vec3 Color(0, 0, 0);

	int Reflections = 1;
	float TempHit = 0, ClosestHit = -1;

	for (int i = 0; i < ObjsSize; i++) {
		TempHit = Objs[i].IntersectionDistance(&R);

		if (TempHit != -1.0f) {
			TempHit = Objs[i].CorrectDistance(&R,TempHit);

			if (TempHit < ClosestHit || ClosestHit == -1) {
				ClosestHit = TempHit;
				Color = Objs[i].Colour * (1.0f / Reflections);
			}
		}
	}

	return Color;
}

void RenderRow(float y, int mStart, SceneObject Objs[], int ObjsSize, unsigned int rgb[]) {
	Ray Rs[ViewWS];

	int i = 0;

	for (float x = -ViewWidth; x < ViewWidth; x += ViewSteps, i++) {
		Rs[i] = Ray(Vec3(0, 0, -20),Vec3(x,y,20));
	}

	array_view<Ray, 1> Rays(ViewWS, Rs);
	array_view<unsigned int, 1> rgb_View(imageMemory, rgb);
	array_view<SceneObject, 1> objs(ObjsSize, Objs);

	parallel_for_each(
		Rays.extent,
		[=](index<1> idx) restrict(amp) {
			Vec3 Color = CalculateRayColour(Rays[idx], objs, ObjsSize);

			idx += mStart;
			idx *= 3;

			rgb_View[idx] = Color.x;
			rgb_View[idx + 1] = Color.y;
			rgb_View[idx + 2] = Color.z;
		}
	);

	rgb = rgb_View.data();
}

unsigned char* RenderScene(SceneObject Objs[]) {
	int i = 0;

	unsigned int rgb [imageMemory];
	unsigned char rgbC[imageMemory];

	for (float y = -ViewWidth; y < ViewWidth; y += ViewSteps, i++) {
		RenderRow(y, i * ViewWS, Objs, 3, rgb);
	}

	for (int j = 0; j < imageMemory; j++) rgbC[j] = rgb[j];

	return rgbC;
}
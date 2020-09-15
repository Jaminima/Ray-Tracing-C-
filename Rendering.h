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
const float ViewSteps = 1.0f;
const int ViewWS = 20;

//12 * pow(ViewWidth / ViewSteps, 2)
const int imageMemory = 1200;

void RenderRow(float y, int mStart, SceneObject Objs[], unsigned int rgb[]) {
	Vec3 Rs[ViewWS] = {};

	int i = 0;

	for (float x = -ViewWidth; x < ViewWidth; x += ViewSteps, i++) {
		Rs[i] = Vec3(x, y, -20);
	}

	array_view<Vec3, 1> Rays(ViewWS, Rs);
	array_view<unsigned int, 1> rgb_View(imageMemory, rgb);

	parallel_for_each(
		Rays.extent,
		[=](index<1> idx) restrict(amp) {
			Vec3 Color = Vec3(255, 0, 0);

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
		RenderRow(y, i * ViewWS, Objs, rgb);
	}

	for (int j = 0; j < imageMemory; j++) rgbC[j] = rgb[j];

	return rgbC;
}
#pragma once
#include "SceneObject.h"

#include "Mesh.h"

#include "List.h"

#include <thread>

#include "vec3.h"

#include <amp.h>
using namespace concurrency;

const int ViewWidth = 10;
const float ViewSteps = 0.01f;
const int ViewWS = 2000;

const int MaxChunkSize = 2000;

//12 * pow(ViewWidth / ViewSteps, 2)
const int imageMemory = 12000000;

Vec3 CalculateRayColour(Ray R, array_view<SceneObject> Objs, int ObjsSize) restrict(amp) {
	Vec3 Color(0, 0, 0);

	int Reflections = 1;
	int ClosestObj=0;

	while (Reflections < 100 && ClosestObj!=-1) {
		float TempHit = 0, ClosestHit = -1;
		ClosestObj = -1;

		for (int i = 0; i < ObjsSize; i++) {
			TempHit = Objs[i].IntersectionDistance(&R);

			if (TempHit != -1.0f) {
				TempHit = Objs[i].CorrectDistance(&R, TempHit);

				if (TempHit < ClosestHit || ClosestHit == -1) {
					ClosestObj = i;
					ClosestHit = TempHit;
				}
			}
		}

		if (ClosestObj != -1) {
			Color += Objs[ClosestObj].Colour * (1.0f / Reflections);

			R = Objs[ClosestObj].PointNormal(Objs[ClosestObj].IntersectionPoint(&R, ClosestHit), &R);
			Reflections++;
		}
	}

	return Color;
}

void RenderRow(float y, int mStart, SceneObject Objs[], int ObjsSize, unsigned char rgb[]) {
	Ray* Rs = (Ray*)malloc(ViewWS*sizeof(Ray));
	unsigned int* rgbTemp = (unsigned int*)malloc(MaxChunkSize*12);

	float x = -ViewWidth;
	int chunkPos = 0, memPos = 0, chunk = 0;

	while (x < ViewWidth) {
		chunkPos = 0;
		for (; x < ViewWidth && chunkPos < MaxChunkSize; x += ViewSteps, chunkPos++, memPos++) {
			Rs[chunkPos] = Ray(Vec3(0, 0, -20), Vec3(x, y, 20));
		}

		array_view<Ray, 1> Rays(ViewWS, Rs);
		array_view<unsigned int, 1> rgb_View(MaxChunkSize*3, rgbTemp);
		array_view<SceneObject, 1> objs(ObjsSize, Objs);

		parallel_for_each(
			Rays.extent,
			[=](index<1> idx) restrict(amp) {
				Vec3 Color = CalculateRayColour(Rays[idx], objs, ObjsSize);

				//idx += mStart;
				idx *= 3;

				rgb_View[idx] = Color.x;
				rgb_View[idx + 1] = Color.y;
				rgb_View[idx + 2] = Color.z;
			}
		);

		std::copy(rgb_View.data(), rgb_View.data() + (chunkPos*3), &rgb[(mStart + memPos - chunkPos) * 3]);

		/*for (int k = 0; k < MaxChunkSize * 3; k++) {
			rgb[k + (mStart+memPos-chunkPos)*3] = rgb_View[k];
		}*/
		
		chunk++;
	}

	free(Rs);
	free(rgbTemp);
}

unsigned char* RenderScene(SceneObject Objs[], int ObjsSize) {
	int i = 0;

	unsigned char* rgb = (unsigned char*)malloc(imageMemory);

	for (float y = -ViewWidth; y < ViewWidth; y += ViewSteps, i++) {
		RenderRow(y, i * ViewWS, Objs, ObjsSize, rgb);
	}

	return rgb;
}
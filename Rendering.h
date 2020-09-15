#pragma once
#include "SceneObject.h"

#include "Sphere.h"
#include "Triangle.h"
#include "Mesh.h"

#include "List.h"

#include <thread>

const int ViewWidth = 10;
const float ViewSteps = 0.01f;
const int ViewWS = 2000;

Vec3 CalculateRayColour(Ray* R, List* Objs, int Reflections = 1) restrict(amp) {
	Vec3 Color(0, 0, 0);
	float HitDistance = -1.0f, THit, rhit;

	Item* I = Objs->Head;
	SceneObject* O;

	while (I != 0x0) {
		O = (SceneObject*)I->Obj;

		if (O != 0x0) {
			THit = O->IntersectionDistance(R);

			if (THit != -1.0f) {
				rhit = O->CorrectDistance(R, THit);

				if (rhit < HitDistance || HitDistance == -1.0f) {
					HitDistance = rhit;

					Color = O->Colour * (1.0f / Reflections);

					if (Reflections < 20) {
						Ray ReflectedRay = O->PointNormal(O->IntersectionPoint(R, HitDistance), R);
						Color += CalculateRayColour(&ReflectedRay, Objs, Reflections + 1);
						//delete ReflectedRay;
					}

				}
			}
		}

		I = I->Next;
	}
	return Color;
}

void RenderRow(float y, int i, List* Objs, unsigned char* rgb) {
	Ray R;
	Vec3 Color;

	for (float x = -ViewWidth; x < ViewWidth; x += ViewSteps, i++) {
		R.Direction = Vec3(x, y, 20);

		Color = CalculateRayColour(R, Objs);

		rgb[i * 3] = Color.x;
		rgb[i * 3 + 1] = Color.y;
		rgb[i * 3 + 2] = Color.z;
	}
}

unsigned char* RenderScene(List* Objs) {
	int i = 0;

	unsigned char* rgb = (unsigned char*)malloc(12 * pow(ViewWidth / ViewSteps, 2));

	std::thread Threads[ViewWS];
	std::thread tThread;

	for (float y = -ViewWidth; y < ViewWidth; y += ViewSteps, i++) {
		tThread = std::thread(RenderRow,y, i * ViewWS, Objs, rgb);
		Threads[i].swap(tThread);
	}

	for (i = 0; i < ViewWS; i++) Threads[i].join();

	return rgb;
}
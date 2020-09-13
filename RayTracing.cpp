#include <iostream>
#include <thread>

#include "SceneObject.h"

#include "Sphere.h"
#include "Triangle.h"

#include "SceneObjectList.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int ViewWidth = 10;
const float ViewSteps = 0.01f;
const int ViewWS = 2000;

Vec3 CalculateRayColour(Ray* R, List* Objs, int Reflections = 1) {
	Vec3 Color(0, 0, 0);
	float HitDistance = -1.0f, THit, rhit;

	Item* I = Objs->Head;
	SceneObject* O;

	while (I != 0x0) {
		O = (SceneObject*)I->Obj;

		THit = O->IntersectionDistance(R);

		if (THit != -1.0f) {
			rhit = O->CorrectDistance(R, THit);

			if (rhit < HitDistance || HitDistance == -1.0f) {
				HitDistance = rhit;

				Color = O->Colour * (1.0f / Reflections);

				if (Reflections < 20) {
					Ray* ReflectedRay = O->PointNormal(O->IntersectionPoint(R, HitDistance), R);
					Color += CalculateRayColour(ReflectedRay, Objs, Reflections + 1);
					delete ReflectedRay;
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

		Color = CalculateRayColour(&R, Objs);

		rgb[i * 3] = Color.x;
		rgb[i * 3 + 1] = Color.y;
		rgb[i * 3 + 2] = Color.z;
	}
}

int main(int argc, char** argv)
{
	List Objs;

	Objs.Add(new Sphere(Vec3(-4, 3, 4), Vec3(255, 0, 0), 4));
	Objs.Add(new Sphere(Vec3(4, 2, -5), Vec3(0, 255, 0), 4));
	Objs.Add(new Sphere(Vec3(0, -5, 2), Vec3(0, 0, 255), 4));
	Objs.Add(new Triangle(Vec3(0,-3,2), Vec3(3,0,-5), Vec3(-3,0,4), Vec3(255, 0, 0)));
	Objs.Add(new Triangle(Vec3(-10, -8, 0), Vec3(-10, 8, 0), Vec3(00, 0, 20), Vec3(0, 120, 0)));

	int i = 0;

	unsigned char* rgb = (unsigned char*)malloc(12 * pow(ViewWidth / ViewSteps, 2));

	std::thread Threads[ViewWS];
	std::thread tThread;

	for (float y = -ViewWidth; y < ViewWidth; y += ViewSteps, i++) {
		tThread = std::thread(RenderRow, y, i * ViewWS, &Objs, rgb);
		Threads[i].swap(tThread);
	}

	for (i = 0; i < ViewWS; i++) Threads[i].join();

	stbi_write_png("image.png", 2 * ViewWidth / ViewSteps, 2 * ViewWidth / ViewSteps, 3, rgb, 0);
}
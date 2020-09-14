#pragma once
#include "Triangle.h"
#include "List.h"

class Mesh : public SceneObject {
public:
	List Triangles;

	Mesh() : SceneObject(Vec3(0, 0, 0)){}

	Mesh(List Triangles) : SceneObject(Vec3(0, 0, 0)) { this->Triangles = Triangles; }

	float CorrectDistance(Ray* Ray, float D) override {
		return (Ray->Origin - IntersectionPoint(Ray, D)).norm() / Ray->Origin.norm();
	}

	bool Intersects(Ray* Ray) override {
		return IntersectionDistance(Ray) != -1;
	}

	SceneObject* HitObject(Ray* Ray) {
		Item* I = Triangles.Head;
		SceneObject* T;
		SceneObject* Closest = 0x0;

		float ClosestDist = -1, TDist;

		while (I != 0x0) {
			T = (SceneObject*)I->Obj;

			TDist = T->IntersectionDistance(Ray);

			if (TDist != -1) {
				TDist = T->CorrectDistance(Ray, TDist);

				if (TDist < ClosestDist || ClosestDist == -1) {
					ClosestDist = TDist;
					Closest = T;
				}
			}

			I = I->Next;
		}

		return Closest;
	}
};
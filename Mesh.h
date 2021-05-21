#pragma once
#include "Triangle.h"
#include "Sphere.h"

class Mesh : public SceneObject {
public:
	Sphere OuterCollider;
	unsigned int triStart = -1, triEnd = -1;

	Mesh() restrict(amp,cpu) {}

	Mesh(unsigned int tri) restrict(cpu) {
		this->triStart = tri;
		sceneTrianglesHead += tri;
		this->triEnd = tri;
		//Triangles = new Triangle[tri];
	}

	Mesh(Triangle* triangles, Triangle* sceneTriangles) restrict(cpu) : Mesh(sizeof(triangles) / sizeof(triangles[0])) {
		for (unsigned int i = triStart;i <= triEnd;i++) {
			sceneTriangles[i] = triangles[i - triStart];
		}
	}

	float RayHitDistance(Ray r, array_view<Triangle, 1> SceneTrianglesView) restrict(amp, cpu) {
		float hit = OuterCollider.RayHitDistance(r);
		if (hit > 0) {
			if (triStart == -1) { return hit; }

			float smallest = -1;
			float t = 0;

			for (unsigned int i = triStart; i <= triEnd;i++) {
				t = SceneTrianglesView[i].RayHitDistance(r);
				if (t != -1 && t < smallest) smallest = t;
			}
			return t;
		}
		return -1;
	}
};
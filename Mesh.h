#pragma once
#include "Triangle.h"
#include "Sphere.h"

class Mesh : public SceneObject {
public:
	Sphere OuterCollider;
	Triangle Triangles[10];
	unsigned int tri = 0;

	Mesh() restrict(amp,cpu) {}

	Mesh(unsigned int tri) restrict(cpu) {
		this->tri = tri;
		//Triangles = new Triangle[tri];
	}

	float RayHitDistance(Ray r) restrict(amp, cpu) {
		float hit = OuterCollider.RayHitDistance(r);
		if (hit > 0) {
			if (tri == 0) { return hit; }

			float smallest = -1;
			float t = 0;

			for (unsigned int i = 0;i < tri;i++) {
				t = Triangles[i].RayHitDistance(r);
				if (t != -1 && t < smallest) smallest = t;
			}
			return t;
		}
		return -1;
	}
};
#pragma once
#include "Triangle.h"

class Mesh : public SceneObject {
public:
	Triangle* Triangles = 0x0;
	unsigned int tri = 0;

	Mesh(unsigned int tri) restrict(cpu) {
		this->tri = tri;
		Triangles = new Triangle[tri];
	}
};
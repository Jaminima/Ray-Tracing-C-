#pragma once
#include "SceneObject.h"
#include "List.h"

class Mesh {
public:
	List Triangles;

	Mesh(){}

	Mesh(List Triangles) { this->Triangles = Triangles; }
};
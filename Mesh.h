#pragma once
#include "Triangle.h"
#include "Sphere.h"

class Mesh : public SceneObject {
public:
	Sphere OuterCollider;
	Vec3 Position;
	int triStart = -1, triEnd = -1;

	Mesh() restrict(amp, cpu) {}

	Mesh(unsigned int tri) restrict(cpu) {
		this->triStart = sceneTrianglesHead;
		sceneTrianglesHead += tri;
		this->triEnd = sceneTrianglesHead;
		//Triangles = new Triangle[tri];
	}

	Vec3 ApproxPosition(Triangle* sceneTriangles) {
		Vec3 Avg;
		for (unsigned int i = triStart;i < triEnd;i++) {
			Avg = Avg + sceneTriangles[i].ApproxPosition();
		}
		Avg = Avg * (1.0f / (triEnd - triStart));
		return Avg + Position;
	}

	bool ImportTriangles(Triangle* triangles, unsigned int tCount, Triangle* sceneTriangles) restrict(cpu) {
		if (triStart != -1) return false;

		this->triStart = sceneTrianglesHead;
		sceneTrianglesHead += tCount;
		this->triEnd = sceneTrianglesHead;

		for (unsigned int i = triStart;i < triEnd;i++) {
			sceneTriangles[i] = triangles[i - triStart];
		}

		return true;
	}

	struct MeshHit {
	public:
		float dist; unsigned int triIDX;
		MeshHit(float d, unsigned int  t) restrict(amp, cpu) { dist = d; triIDX = t; }
		MeshHit() restrict(amp, cpu) { dist = -1; triIDX = 0; }
	};

	MeshHit RayHitDistance(Ray r, array_view<Triangle, 1> SceneTrianglesView) restrict(amp, cpu) {
		r.Origin = r.Origin - Position;
		float hit = OuterCollider.RayHitDistance(r);
		if (hit != -1 || (OuterCollider.Center - r.Origin).norm() < OuterCollider.radius) {
			if (triStart == -1) { return MeshHit(); }

			float smallest = -1;
			unsigned int triIDX = 0;
			float t = 0;

			for (unsigned int i = triStart; i < triEnd;i++) {
				t = SceneTrianglesView[i].RayHitDistance(r);
				if (t != -1 && (t < smallest || smallest == -1)) { smallest = t; triIDX = i; }
			}
			return MeshHit(smallest, triIDX);
		}
		return MeshHit();
	}
};
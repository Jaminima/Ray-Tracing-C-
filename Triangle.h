#pragma once
#include "SceneObject.h"

const float EPSILON = 0.0000001;

class Triangle : public SceneObject {
public:
	Vec3 V0, V1, V2;

	Triangle(Vec3 V0, Vec3 V1, Vec3 V2, Vec3 Col) : SceneObject(Col) {
		this->V0 = V0; this->V1 = V1; this->V2 = V2;

		this->V0.y *= -1;
		this->V1.y *= -1;
		this->V2.y *= -1;
	}

	float CorrectDistance(Ray* Ray, float D) override {
		return (Ray->Origin-IntersectionPoint(Ray, D)).norm()/Ray->Origin.norm();
	}

	bool Intersects(Ray* Ray) override {
		return IntersectionDistance(Ray) != -1;
	}

	float IntersectionDistance(Ray* Ray) override {
		Vec3 E1 = V1 - V0,
			E2 = V2 - V0;

		Vec3 h = Ray->Direction.cross(E2);

		float a = E1.dot(h);

		if (a > -EPSILON && a < EPSILON) return -1;

		float f = 1.0f / a;
		Vec3 s = Ray->Origin - V0;
		float u = f * s.dot(h);

		if (u < 0 || u>1) return -1;

		Vec3 q = s.cross(E1);
		float v = f * Ray->Direction.dot(q);

		if (v < 0 || u + v > 1) return -1;

		float t = f * E2.dot(q);
		if (t > EPSILON)
		{
			return t;
		}
		else return -1;
	}

	Ray* PointNormal(Vec3 Point, Ray* oRay) override {
		Ray* nRay = new Ray();

		Vec3 V = V1 - V0, W = V2 - V0;

		Vec3 N(V.y * V.z - V.z * W.y, V.z * W.x - V.x * W.z, V.x * W.y - V.y * W.x);

		Ray oCopy;
		oCopy.Direction = 1 * oRay->Direction;
		oCopy.Origin = 1 * oRay->Origin;

		oCopy.Direction.normalise();

		nRay->Direction = - (oCopy.Direction + 2*(oCopy.Direction.mul(N).mul(V)));
		nRay->Origin = Point + (nRay->Direction*EPSILON);

		return nRay;
	}
};
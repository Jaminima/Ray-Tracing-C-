#pragma once
#include "vec3.h"

class Ray {
public:
	Vec3 Origin = Vec3(0, 0, -20), Direction;

	Ray() {}
	Ray() restrict(amp) {}

	Ray(Vec3 O, Vec3 D) { Origin = O; Direction = D; }
	Ray(Vec3 O, Vec3 D) restrict(amp) { Origin = O; Direction = D; }
};

enum Type {
	Sphere,
	Triangle
};

class SceneObject {
public:
	Vec3 Colour;

	Type T;

	SceneObject(Vec3 Col) {
		this->Colour = Col;
	}

#pragma region Sphere
	Vec3 Center;

	float radius = 0;

	SceneObject(Vec3 Cent, Vec3 Col, float r) : SceneObject(Col) {
		T = (Type)0;

		this->Center = Cent;

		this->Center.y *= -1;

		this->radius = r;
	}

	bool Sphere_Intersects(Ray* Ray) restrict(amp) {
		return IntersectionDistance(Ray) >= 0;
	}

	float Sphere_IntersectionDistance(Ray* Ray) restrict(amp) {
		Vec3 oc = Ray->Origin - Center;

		float a = Ray->Direction.dot(Ray->Direction),
			b = 2 * oc.dot(Ray->Direction),
			c = oc.dot(oc) - radius * radius;

		float discriminant = (b * b - 4 * a * c);

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			float d = (-b - concurrency::fast_math::sqrtf(discriminant)) / (2.0 * a);
			if (d < 0) return -1.0;
			else return d;
		}
	}

	Ray Sphere_PointNormal(Vec3 Point, Ray* oRay) restrict(amp) {
		Ray nRay;

		nRay.Direction = (((Point - Center) * oRay->Direction.dot(Point - Center)) * 2 - oRay->Direction) * -1;
		nRay.Origin = Point;

		return nRay;
	}
#pragma endregion

#pragma region Triangle
	Vec3 V0, V1, V2;

	SceneObject(Vec3 V0, Vec3 V1, Vec3 V2, Vec3 Col) : SceneObject(Col) {
		T = (Type)1;

		this->V0 = V0; this->V1 = V1; this->V2 = V2;

		this->V0.y *= -1;
		this->V1.y *= -1;
		this->V2.y *= -1;
	}

	float Triangle_CorrectDistance(Ray* Ray, float D) restrict(amp) {
		return (Ray->Origin - IntersectionPoint(Ray, D)).norm() / Ray->Origin.norm();
	}

	bool Triangle_Intersects(Ray* Ray) restrict(amp) {
		return IntersectionDistance(Ray) != -1;
	}

	float Triangle_IntersectionDistance(Ray* Ray) restrict(amp) {
		Vec3 E1 = V1 - V0,
			E2 = V2 - V0;

		Vec3 h = Ray->Direction.cross(E2);

		float a = E1.dot(h);

		if (a > -0.0000001 && a < 0.0000001) return -1;

		float f = 1.0f / a;
		Vec3 s = Ray->Origin - V0;
		float u = f * s.dot(h);

		if (u < 0 || u>1) return -1;

		Vec3 q = s.cross(E1);
		float v = f * Ray->Direction.dot(q);

		if (v < 0 || u + v > 1) return -1;

		float t = f * E2.dot(q);
		if (t > 0.0000001)
		{
			return t;
		}
		else return -1;
	}

	Ray Triangle_PointNormal(Vec3 Point, Ray* oRay) restrict(amp) {
		Ray nRay;

		Vec3 V = V1 - V0, W = V2 - V0;

		Vec3 N(V.y * V.z - V.z * W.y, V.z * W.x - V.x * W.z, V.x * W.y - V.y * W.x);

		Ray oCopy;
		oCopy.Direction = oRay->Direction * 1;
		oCopy.Origin = oRay->Origin * 1;

		oCopy.Direction.normalise();

		nRay.Direction = (oCopy.Direction + (oCopy.Direction * N * V) * 2) * -1;
		nRay.Origin = Point + (nRay.Direction * 0.0000001f);

		return nRay;
	}
#pragma endregion

	float CorrectDistance(Ray* Ray, float D) restrict(amp) {
		if (T == 1) return Triangle_CorrectDistance(Ray, D);
		return D;
	}

	bool Intersects(Ray* Ray) restrict(amp) {
		if (T == 0) return Sphere_Intersects(Ray);
		if (T == 1) return Triangle_Intersects(Ray);
	}

	float IntersectionDistance(Ray* Ray) restrict(amp) {
		if (T == 0) return Sphere_IntersectionDistance(Ray);
		if (T == 1) return Triangle_IntersectionDistance(Ray);
	}

	Ray PointNormal(Vec3 Point, Ray* oRay) restrict(amp) {
		if (T == 0) return Sphere_PointNormal(Point, oRay);
		if (T == 1) return Triangle_PointNormal(Point, oRay);
	}

	Vec3 IntersectionPoint(Ray* Ray, float Distance) restrict(amp) {
		return Ray->Origin + (Ray->Direction * Distance);
	}
};

#pragma once
#include "vec3.h"

class Ray {
public:
	Vec3 Origin = Vec3(0,0,-20), Direction;
};

class SceneObject {
public:
	Vec3 Center;
	Vec3 Colour;

	bool Intersects(Ray* Ray);
};

class Sphere : SceneObject {
public:
	float radius = 0;

	bool Intersects(Ray* Ray) {

		return IntersectionDistance(Ray)>=0;
	}

	float IntersectionDistance(Ray* Ray) {
		Vec3 oc = Ray->Origin - Center;

		float a = Ray->Direction.dot(Ray->Direction),
			b = 2 * oc.dot(Ray->Direction),
			c = oc.dot(oc) - radius * radius;

		float discriminant = (b * b - 4 * a * c);

		if (discriminant < 0) {
			return -1.0;
		}
		else {
			float d = (-b - sqrt(discriminant)) / (2.0 * a);
			if (d < 0) return -1.0;
			else return d;
		}
	}

	Vec3 IntersectionPoint(Ray* Ray, float Distance) {
		return Ray->Origin + (Ray->Direction * Distance);
	}

	Ray* PointNormal(Vec3 Point, Ray* oRay) {

		Ray* nRay = new Ray();

		nRay->Direction = -(2 * (oRay->Direction.dot(Point - Center)) * (Point - Center) - oRay->Direction);
		nRay->Origin = Point;

		return nRay;
	}

	void SetColor(Vec3 Color) {
		this->Colour = Color;
	}

	Vec3 GetColor() {
		return Colour;
	}

	void SetCenter(Vec3 NewPos) {
		Center = NewPos;
	}
};




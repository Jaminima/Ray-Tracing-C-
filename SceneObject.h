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

//class SceneObject {
//public:
//	Vec3 Colour;
//
//	Type T;
//
//	SceneObject(Vec3 Col) {
//		this->Colour = Col;
//	}
//
//	Vec3 Center;
//
//	float radius = 0;
//
//	SceneObject(Vec3 Cent, Vec3 Col, float r) : SceneObject(Col) {
//		this->Center = Cent;
//
//		this->Center.y *= -1;
//
//		this->radius = r;
//	}
//
//	bool Sphere_Intersects(Ray* Ray) restrict(amp) {
//		return Sphere_IntersectionDistance(Ray) >= 0;
//	}
//
//	float Sphere_IntersectionDistance(Ray* Ray) restrict(amp) {
//		Vec3 oc = Ray->Origin - Center;
//
//		float a = Ray->Direction.dot(Ray->Direction),
//			b = 2 * oc.dot(Ray->Direction),
//			c = oc.dot(oc) - radius * radius;
//
//		float discriminant = (b * b - 4 * a * c);
//
//		if (discriminant < 0) {
//			return -1.0;
//		}
//		else {
//			float d = (-b - concurrency::fast_math::sqrtf(discriminant)) / (2.0 * a);
//			if (d < 0) return -1.0;
//			else return d;
//		}
//	}
//
//	Ray Sphere_PointNormal(Vec3 Point, Ray* oRay) restrict(amp) {
//		Ray nRay;
//
//		nRay.Direction = (((Point - Center) * oRay->Direction.dot(Point - Center)) * 2 - oRay->Direction) * -1;
//		nRay.Origin = Point;
//
//		return nRay;
//	}
//
//	Vec3 IntersectionPoint(Ray* Ray, float Distance) restrict(amp) {
//		return Ray->Origin + (Ray->Direction * Distance);
//	}
//};

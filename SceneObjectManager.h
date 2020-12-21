#pragma once
#include "Triangle.h"
#include "Sphere.h"

enum ObjectType {
	None,
	_Sphere,
	_Triangle
};

class SceneObjectManager {
private:
	ObjectType type = None;

	Sphere _sphere;
	Triangle _triangle;

public:
	float reflectivity() restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.reflectivity;
			break;
		case _Triangle:
			return _triangle.reflectivity;
			break;
		}
	}

	Color color() restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.color;
			break;
		case _Triangle:
			return _triangle.color;
			break;
		}
	}

	SceneObjectManager() {}

	void SetTriangle(Triangle _tri) {
		_triangle = _tri;
		type = _Triangle;
	}

	void SetSphere(Sphere _sph) {
		_sphere = _sph;
		type = _Sphere;
	}

	Vec3 ApproxPosition() restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.ApproxPosition();
			break;
		case _Triangle:
			return _triangle.ApproxPosition();
			break;
		}
	}

	float RayHitDistance(Ray r) restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.RayHitDistance(r);
			break;
		case _Triangle:
			return _triangle.RayHitDistance(r);
			break;
		}
	}

	float CorrectDistance(Ray r, float d) restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.CorrectDistance(r, d);
			break;
		case _Triangle:
			return _triangle.CorrectDistance(r, d);
			break;
		}
	}

	bool RayHit(Ray r) restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.RayHit(r);
			break;
		case _Triangle:
			return _triangle.RayHit(r);
			break;
		}
	}

	Vec3 IntersectionPoint(Ray r, float Distance) restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.IntersectionPoint(r, Distance);
			break;
		case _Triangle:
			return _triangle.IntersectionPoint(r, Distance);
			break;
		}
	}

	Ray PointNormal(Vec3 Point, Ray oRay) restrict(amp) {
		switch (type)
		{
		case _Sphere:
			return _sphere.PointNormal(Point, oRay);
			break;
		case _Triangle:
			return _triangle.PointNormal(Point, oRay);
			break;
		}
	}
};
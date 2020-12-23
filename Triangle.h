#pragma once
#include "SceneObject.h"

class Triangle : public SceneObject
{
public:
	Vec3 Corners[3]{ Vec3(0, 0, 5), Vec3(5, 5, 0), Vec3(10, 0, -5) }; //Triangle

	Triangle() restrict(amp, cpu)
	{
	}

	Triangle(Vec3 Corners[3]) restrict(amp, cpu)
	{
		this->Corners[0] = Corners[0];
		this->Corners[1] = Corners[1];
		this->Corners[2] = Corners[2];
	}

	Triangle(Vec3 Corners[3], Color c) restrict(amp, cpu) : Triangle(Corners)
	{
		this->color = c;
	}

	Triangle(Vec3 Corners[3], Color c, float reflectivity, float opacity) restrict(amp, cpu) : Triangle(Corners)
	{
		this->color = c;
		this->opacity = opacity;
		this->reflectivity = reflectivity;
	}

	Triangle(Vec3 c1, Vec3 c2, Vec3 c3) restrict(amp, cpu)
	{
		this->Corners[0] = c1;
		this->Corners[1] = c2;
		this->Corners[2] = c3;
	}

	Triangle(Vec3 c1, Vec3 c2, Vec3 c3, Color c) restrict(amp, cpu)
	{
		this->Corners[0] = c1;
		this->Corners[1] = c2;
		this->Corners[2] = c3;
		this->color = c;
	}

	Triangle(Vec3 c1, Vec3 c2, Vec3 c3, Color c, float reflectivity, float opacity) restrict(amp, cpu)
	{
		this->Corners[0] = c1;
		this->Corners[1] = c2;
		this->Corners[2] = c3;
		this->color = c;
		this->opacity = opacity;
		this->reflectivity = reflectivity;
	}

	Vec3 ApproxPosition() restrict(amp, cpu)
	{
		return (Corners[0] + Corners[1] + Corners[2]) * (1.0f / 3);
	}

	float RayHitDistance(Ray r) restrict(amp, cpu)
	{
		Vec3 E1 = Corners[1] - Corners[0], E2 = Corners[2] - Corners[0];

		Vec3 h = r.Direction.cross(E2);

		float a = E1.dot(h);

		if (a > -0.0000001 && a < 0.0000001) return -1;

		float f = 1.0f / a;
		Vec3 s = r.Origin - Corners[0];
		float u = f * s.dot(h);

		if (u < 0 || u > 1) return -1;

		Vec3 q = s.cross(E1);
		float v = f * r.Direction.dot(q);

		if (v < 0 || u + v > 1) return -1;

		float t = f * E2.dot(q);
		if (t > 0.0000001)
		{
			return t;
		}
		return -1;
	}

	/*float CorrectDistance(Ray r, float d) restrict(amp, cpu) {
		return (r.Origin - IntersectionPoint(r, d)).norm() / r.Origin.norm();
	}*/

	bool RayHit(Ray r) restrict(amp, cpu)
	{
		return RayHitDistance(r) != -1;
	}

	Ray PointNormal(Vec3 Point, Ray oRay) restrict(amp, cpu)
	{
		Ray nRay;

		Vec3 V = Corners[1] - Corners[0], W = Corners[2] - Corners[0];

		Vec3 N(V.y * V.z - V.z * W.y, V.z * W.x - V.x * W.z, V.x * W.y - V.y * W.x);

		nRay.Direction = oRay.Direction * 1;

		nRay.Direction.normalise();

		nRay.Direction = (nRay.Direction + (nRay.Direction * N * V) * 2) * -1;
		nRay.Origin = Point /*+ (nRay.Direction * 0.0000001f)*/;

		return nRay;
	}
};

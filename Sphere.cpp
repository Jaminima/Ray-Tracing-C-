#include "SceneObject.h"

float Sphere::IntersectionDistance(Ray* Ray) {
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
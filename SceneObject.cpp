//#include "SceneObject.h"
//
//bool SceneObject::Intersects(Ray* Ray) {
//	return false;
//}
//
//bool Sphere::Intersects(Ray* Ray) {
//
//	/*float dX = Ray->cX - Ray->vX;
//	float dY = Ray->cY - Ray->vY;
//	float dZ = Ray->cZ - Ray->vZ;
//
//	float a = dX * dX + dY * dY + dZ * dZ;
//
//	float b = 2 * (dX * (Ray->vX - sX) + dX * (Ray->vY - sY) + dX * (Ray->vZ - sZ));
//
//	float c = sX * sX + sY * sY + sZ * sZ + Ray->vX * Ray->vX + Ray->vY * Ray->vY + Ray->vZ * Ray->vZ - 2 * (sX * Ray->cX + sY * Ray->cY + sZ * Ray->cZ) - radius*radius;
//
//	float s = b * b - (4 * a * c);
//
//	return s>=0;*/
//
//	Vec3 oc = Ray->Origin - Center;
//
//	float a = Ray->Direction.dot(Ray->Direction),
//		b = 2*oc.dot(Ray->Direction),
//		c = oc.dot(oc) - radius*radius;
//	
//	return (b * b - 4 * a * c)>0;
//}
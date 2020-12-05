#pragma once
//#include "List.h"
#include "Sphere.h"

#include "Const.h"

#include <Windows.h>
#include <amp.h>
using namespace concurrency;

#include "GL/glut.h"
#include "GL/freeglut.h"

Color RenderRay(Ray r, array_view<Sphere, 1> spheres) restrict(amp) {
	Color c(0, 0, 0);
	float LastHit;
	Ray hitRay;
	int hitSphere = 0;
	unsigned int reflection = 1;

	while (hitSphere != -1) {
		hitSphere = -1;
		LastHit = 0xFFFFFFFF;

		for (unsigned int i = 0; i < spheres.extent.size(); i++) {
			float RayHit = spheres[i].RayHitDistance(r);

			if (RayHit > 0 && RayHit < LastHit) {
				hitSphere = i;
				hitRay = r;
				LastHit = RayHit;
			}
		}

		if (hitSphere != -1) {
			c = c + (spheres[hitSphere].color * (1.0f/reflection));
			r = spheres[hitSphere].Sphere_PointNormal(spheres[hitSphere].IntersectionPoint(&hitRay, LastHit), &hitRay);
		}

		reflection++;
	}

	return c;
}

Color RenderPixel(index<2> idx, array_view<Sphere,1> spheres, Vec3 Camera) restrict(amp) {
	float vx = (idx[1] / (float)px_half) - 1,
		vy = (idx[0] / (float)px_half) - 1;

	Ray r(Camera,Vec3(vx, vy, 1));
	return RenderRay(r,spheres);
}

Color* RenderScene(Color* rgb) {
	Sphere spheres[3]{ Sphere(), Sphere()};

	spheres[0].color = Color(0, 255, 0);

	spheres[1].Center = Vec3(5, 5, 5);
	spheres[1].color = Color(255, 0, 0);

	spheres[2].Center = Vec3(-5, -5, 5);
	spheres[2].color = Color(0,0,255);

	array_view<Color, 2> ColorView(px, py, rgb);
	array_view<Sphere, 1> SphereView(3, spheres);

	Vec3 Camera = Vec3(mainCamera);

	parallel_for_each(
		ColorView.extent,
		[=](index<2> idx) restrict(amp) {
			ColorView[idx[0]][idx[1]] = RenderPixel(idx, SphereView, Camera);
		}
	);

	ColorView.synchronize();

	return rgb;
}

Color* rgb;

void drawFrame()
{
	glDrawPixels(px, py, GL_RGBA, GL_UNSIGNED_BYTE, rgb);
	glutSwapBuffers();
}

void triggerReDraw() {
	mainCamera.x += 0.01f;

	DWORD start = timeGetTime();
	RenderScene(rgb);
	DWORD end = timeGetTime();

	printf_s("Frame took %d ms IE %d FPS\n", (end-start), 1000/(end-start));
	glutPostRedisplay();
}

void SetupFrame(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitWindowSize(px, py);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("glDrawPixels example");

    glutDisplayFunc(drawFrame);
	glutIdleFunc(triggerReDraw);

    glutMainLoop();
}
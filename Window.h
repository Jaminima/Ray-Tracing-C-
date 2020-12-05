#pragma once
#include <Windows.h>
#include "Rendering.h"

#include <chrono>
using namespace std::chrono;

Color* rgb;

void drawFrame()
{
	glDrawPixels(px, py, GL_RGBA, GL_UNSIGNED_BYTE, rgb);
	glutSwapBuffers();
}

void triggerReDraw() {
	//mainCamera.Angle.x -= 0.001f;
	//spheres[0].Center.z+= 0.01f;

	high_resolution_clock::time_point before = high_resolution_clock::now();

	RenderScene(rgb);

	glutPostRedisplay();

	high_resolution_clock::time_point after = high_resolution_clock::now();
	duration<double> ns = after - before;

	printf_s("Frame took %d ns\n", ns);
}

void SetupFrame(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(px, py);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("glDrawPixels example");

	glutDisplayFunc(drawFrame);
	glutIdleFunc(triggerReDraw);

	glutMainLoop();
}
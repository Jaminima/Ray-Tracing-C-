#pragma once
#include <Windows.h>
#include "Rendering.h"
#include "Aliasing.h"

Color* rgb;

void drawFrame()
{
	glDrawPixels(px, py, GL_RGBA, GL_UNSIGNED_BYTE, rgb);
	glutSwapBuffers();
}

void triggerReDraw() {
	//mainCamera.Angle.x -= 0.001f;
	//spheres[0].Center.z+= 0.01f;

	clock_t start = clock();

	RenderScene(rgb);
	FXAA(rgb);

	glutPostRedisplay();

	clock_t end = clock();

	clock_t ms = end - start;

	printf_s("Frame took %d ms\n", ms);
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

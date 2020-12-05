#pragma once
#include <Windows.h>
#include "Rendering.h"
#include "Aliasing.h"

Color* rgb;

unsigned int framesInSec = 0;
time_t startTime = clock();

void drawFrame()
{
	glDrawPixels(px, py, GL_RGBA, GL_UNSIGNED_BYTE, rgb);
	glutSwapBuffers();
}

void triggerReDraw() {
	//mainCamera.Angle.x -= 0.001f;
	mainCamera.Position.z += 0.01f;

	framesInSec++;

	RenderScene(rgb);
	//FXAA(rgb);

	glutPostRedisplay();

	if (clock() - startTime >= 1000) {
		printf_s("You averaged %d fps\n", framesInSec);
		framesInSec = 0;
		startTime = clock();
	}
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

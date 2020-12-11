#pragma once
#include <Windows.h>
#include "Rendering.h"
#include "Aliasing.h"

Color* rgbBuffers = (Color*)malloc(2 * px * py * sizeof(Color));

array_view<Color, 3> rgb(2, py, px, rgbBuffers);

bool LockedBuffer = false;
unsigned int framesInSec = 0;
time_t startTime = clock();

void drawFrame()
{
	glDrawPixels(px, py, GL_RGBA, GL_UNSIGNED_BYTE, &rgbBuffers[px * py * !LockedBuffer]);
	glutSwapBuffers();
}

Concurrency::completion_future pendingFrameCopy;

void triggerReDraw() {
	//spheres[2].Center.x += 0.01f;
	mainCamera.Position.z += 0.01f;
	//mainCamera.Position.x += 0.01f;
	mainCamera.RotateCamera(Vec3(0, -0.001f, 0));

	framesInSec++;

	OrderCamera();

	RenderScene(rgb[LockedBuffer]);
	FXAA(rgb[LockedBuffer]);

	pendingFrameCopy = rgb[LockedBuffer].synchronize_async();

	glutPostRedisplay();

	LockedBuffer = !LockedBuffer;

	if (clock() - startTime >= 1000) {
		printf_s("You averaged %d fps\n", framesInSec);
		framesInSec = 0;
		startTime = clock();
	}

	pendingFrameCopy.wait();
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

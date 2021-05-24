#pragma once
#include <Windows.h>
#include "Rendering.h"
#include "Aliasing.h"
#include "Inputs.h"

#include "GL/glut.h"
#include "GL/freeglut.h"

Color* rgbBuffers = static_cast<Color*>(malloc(2 * px * py * sizeof(Color)));

array_view<Color, 3> rgb(2, py, px, rgbBuffers);

bool LockedBuffer = false;
unsigned int framesInSec = 0;
time_t startTime = clock();

void drawFrame()
{
	glDrawPixels(px, py, GL_RGBA, GL_UNSIGNED_BYTE, &rgbBuffers[px * py * !LockedBuffer]);
	glutSwapBuffers();
}

completion_future pendingFrameCopy;

void triggerReDraw()
{
	framesInSec++;

	pendingFrameCopy = rgb[LockedBuffer].synchronize_async();

	RenderScene(rgb[!LockedBuffer]);
	FXAA(rgb[!LockedBuffer]);

	if (clock() - startTime >= 1000)
	{
		printf_s("You averaged %d fps\n", framesInSec);
		framesInSec = 0;
		startTime = clock();
	}

	glutPostRedisplay();

	LockedBuffer = !LockedBuffer;

	pendingFrameCopy.wait();
}

void SetupFrame(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(px, py);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("glDrawPixels example");

	glutDisplayFunc(drawFrame);
	glutIdleFunc(triggerReDraw);

	glutPassiveMotionFunc(MouseMove);
	glutKeyboardFunc(KeyboardDepressed);

	glutMainLoop();
}

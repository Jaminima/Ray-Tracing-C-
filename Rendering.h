#pragma once
//#include "List.h"
#include "Sphere.h"

#include "Const.h"

#include <Windows.h>
#include <amp.h>
using namespace concurrency;

#include "GL/glut.h"
#include "GL/freeglut.h"

Color* RenderScene(Color* rgb) {
	array_view<Color, 2> ColorView(px, py, rgb);

	parallel_for_each(
		ColorView.extent,
		[=](index<2> idx) restrict(amp) {
			ColorView[idx[0]][idx[1]] = Sphere().RenderRay(idx);
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
	RenderScene(rgb);
	std::cout << "New Frame\n";
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
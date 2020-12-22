#pragma once
#include "Const.h"
#include "GL/glut.h"
#include "GL/freeglut.h"

void KeyboardDepressed(unsigned char key, int x, int y) {
	Vec3 camMove(0, 0, 0);

	if (key == 'w') camMove.z += camMoveStep;
	if (key == 's') camMove.z -= camMoveStep;

	if (key == 'a') camMove.x -= camMoveStep;
	if (key == 'd') camMove.x += camMoveStep;

	if (key == 'q') exit(0);

	mainCamera.MoveCamera(camMove);
}

void MouseMove(int x, int y) {
	int movex = px_half - x, movey = py_half - y;

	mainCamera.RotateCamera(Vec3(movey * camSensitvity, -movex * camSensitvity, 0));

	glutWarpPointer(px_half, py_half);
}
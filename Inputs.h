#pragma once
#include "Const.h"

void KeyboardDepressed(unsigned char key, int x, int y) {
	Vec3 camMove(0,0,0);

	if (key == 'w') camMove.z += camMoveStep;
	if (key == 's') camMove.z -= camMoveStep;

	if (key == 'a') camMove.x -= camMoveStep;
	if (key == 'd') camMove.x += camMoveStep;

	mainCamera.MoveCamera(camMove);
}
#pragma once
#include "Camera.h"

const unsigned int px = 1920, py = 1080;
const unsigned int px_half = px / 2.0f, py_half = py / 2.0f;

const unsigned int totalSceneObjects = 5, totalLights = 2;

const int reflectionLimit = 100, lightHitLimit = 100;

extern Camera mainCamera;

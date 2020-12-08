#pragma once
#include "Camera.h"

const unsigned int px = 1920, py = 1080;
const unsigned int px_half = px / 2.0f, py_half = py / 2.0f;

const unsigned int totalSpheres = 4, totalLights = 2;

const int reflectionLimit = 5, lightHitLimit = 2;

Camera mainCamera;

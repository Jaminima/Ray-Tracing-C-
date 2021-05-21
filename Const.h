#pragma once
#include "Camera.h"
#include "Color.h"

#ifndef __Const
#define __Const

//const unsigned int px = 720, py = 480;
const unsigned int px = 1920, py = 1080;

const unsigned int px_half = px / 2.0f, py_half = py / 2.0f;

const unsigned int totalSceneObjects = 1, totalLights = 2, totalSceneTriangles = 100;
unsigned int sceneTrianglesHead = 0;

const int reflectionLimit = 100, lightHitLimit = 100;

const bool runOnCpu = false;

const Vec3 WorldLight(0.8f, 0.8f, 0.8f);

const Color WorldColor(200, 200, 200);

extern Camera mainCamera;

const float camMoveStep = 0.1f, camSensitvity = 0.001f;

#endif // !__Const

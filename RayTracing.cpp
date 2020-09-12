#include <iostream>
#include "SceneObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int ViewWidth = 10;
const float ViewSteps = 0.01f;

Vec3 CalculateRayColour(Ray* R, Sphere Spheres[], int Reflections = 1) {
    Vec3 Color(0, 0, 0);
    float HitDistance = -1.0f, THit;

    for (int i = 0; i < 3; i++) {

        THit = Spheres[i].IntersectionDistance(R);

        if (THit != -1.0f && (THit < HitDistance || HitDistance==-1.0f)) {
            HitDistance = THit;

            Color = Spheres[i].GetColor()*(1.0f/Reflections);

            Ray* ReflectedRay = Spheres[i].PointNormal(Spheres[i].IntersectionPoint(R, HitDistance), R);

            if (Reflections<10) 
                Color += CalculateRayColour(ReflectedRay, Spheres, Reflections + 1);

            delete ReflectedRay;
        }

    }
    return Color;
}


int main(int argc, char** argv)
{
    Sphere Spheres[3]{ Sphere(), Sphere(), Sphere() };

    Spheres[0].radius = 4; Spheres[0].SetCenter(Vec3(-4, -3, 4)); Spheres[0].SetColor(Vec3(255, 0, 0));
    Spheres[1].radius = 4; Spheres[1].SetCenter(Vec3(4, -2, -1)); Spheres[1].SetColor(Vec3(0, 255, 0));
    Spheres[2].radius = 4; Spheres[2].SetCenter(Vec3(0, 5, 2)); Spheres[2].SetColor(Vec3(0, 0, 255));

    int i = 0;

    unsigned char* rgb = (unsigned char*) malloc((2*ViewWidth/ViewSteps)*(2*ViewWidth/ViewSteps)*3);

    Ray R;
    for (float x = -ViewWidth, y=-ViewWidth; y < ViewWidth; x += ViewSteps,i++) {
        if (x >= ViewWidth) { x = -ViewWidth; y += ViewSteps; }
        R.Direction = Vec3(x, y, 20);

        Vec3 Color = CalculateRayColour(&R, Spheres);

        rgb[i * 3] = Color.x;
        rgb[i * 3 + 1] = Color.y;
        rgb[i * 3 + 2] = Color.z;
    }

    stbi_write_png("image.png", 2*ViewWidth/ViewSteps, 2*ViewWidth/ViewSteps, 3, rgb, 0);
}


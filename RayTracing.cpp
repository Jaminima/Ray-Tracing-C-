#include <iostream>
#include "SceneObject.h"
#include "Sphere.h"
#include "SceneObjectList.h"

#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int ViewWidth = 10;
const float ViewSteps = 0.01f;
const int ViewWS = 2000;

Vec3 CalculateRayColour(Ray* R, List* Objs, int Reflections = 1) {
    Vec3 Color(0, 0, 0);
    float HitDistance = -1.0f, THit;

    Item* I = Objs->Head;
    SceneObject* O;

    while (I!=0x0){
        O = (SceneObject*)I->Obj;

        THit = O->IntersectionDistance(R);

        if (THit != -1.0f && (THit < HitDistance || HitDistance==-1.0f)) {
            HitDistance = THit;

            Color = O->Colour*(1.0f/Reflections);

            Ray* ReflectedRay = O->PointNormal(O->IntersectionPoint(R, HitDistance), R);

            if (Reflections<10) 
                Color += CalculateRayColour(ReflectedRay, Objs, Reflections + 1);

            delete ReflectedRay;
        }

        I = I->Next;
    }
    return Color;
}

void RenderRow(float y, int i, List* Objs, unsigned char* rgb) {
    
    for (float x = -ViewWidth; x < ViewWidth; x += ViewSteps, i++) {
        Ray R;

        R.Direction = Vec3(x, y, 20);

        Vec3 Color = CalculateRayColour(&R, Objs);

        rgb[i * 3] = Color.x;
        rgb[i * 3 + 1] = Color.y;
        rgb[i * 3 + 2] = Color.z;
    }
}

int main(int argc, char** argv)
{
    List Objs;

    Objs.Add(new Sphere(Vec3(-4, 3, 4), Vec3(255, 0, 0), 4));
    Objs.Add(new Sphere(Vec3(4, 2, -1), Vec3(0, 255, 0), 4));
    Objs.Add(new Sphere(Vec3(0, -5, 2), Vec3(0, 0, 255), 4));

    int i = 0;

    unsigned char* rgb = (unsigned char*)malloc((2 * ViewWidth / ViewSteps) * (2 * ViewWidth / ViewSteps) * 3);

    std::thread Threads[ViewWS];
    std::thread tThread;

    for (float y = -ViewWidth; y < ViewWidth; y+=ViewSteps, i++) {
        tThread = std::thread(RenderRow, y, i * ViewWS, &Objs, rgb);
        Threads[i].swap(tThread);
    }

    stbi_write_png("image.png", 2*ViewWidth/ViewSteps, 2*ViewWidth/ViewSteps, 3, rgb, 0);
}


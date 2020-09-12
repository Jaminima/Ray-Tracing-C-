#include <iostream>
#include "SceneObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int ViewWidth = 10;
const float ViewSteps = 0.01f;

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

    Sphere S;
    S.radius = 4;

    Sphere S2;
    S2.radius = 4;
    S2.SetCenter(Vec3(3,1,0));

    int i = 0;
    //bool grid[441];

    unsigned char* rgb = (unsigned char*) malloc((2*ViewWidth/ViewSteps)*(2*ViewWidth/ViewSteps)*3);

    Ray R;
    for (float x = -ViewWidth, y=-ViewWidth; y < ViewWidth; x += ViewSteps,i++) {
        if (x >= ViewWidth) { x = -ViewWidth; y += ViewSteps; }
        R.Direction = Vec3(x, y, 20);

        //grid[i] = S.Intersects(&R);

        if (S2.Intersects(&R)) {
            rgb[i * 3] = 0;
            rgb[(i * 3) + 1] = 255;
            rgb[(i * 3) + 2] = 0;
        }

        if (S.Intersects(&R)) {
            rgb[i * 3] = 255;
            rgb[(i * 3) + 1] = 0;
            rgb[(i * 3) + 2] = 0;
        }
    }

    stbi_write_png("image.png", 2*ViewWidth/ViewSteps, 2*ViewWidth/ViewSteps, 3, rgb, 0);
}

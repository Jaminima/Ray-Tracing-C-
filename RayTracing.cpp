#include <iostream>
#include "SceneObject.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

    Sphere S;
    S.radius = 6;

    Sphere S2;
    S2.radius = 4;
    S2.SetCenter(Vec3(0,0,0));

    int i = 0;
    bool grid[441];

    unsigned char* rgb = (unsigned char*) malloc(20*20*3);

    Ray R;
    for (float x = -10, y=-10; y < 10; x += 1,i++) {
        if (x >= 10) { x = -10; y += 1; }
        R.Direction = Vec3(x, y, 20);

        //grid[i] = S.Intersects(&R);

        if (S2.Intersects(&R)) {
            rgb[i * 3] = 0;
            rgb[(i * 3) + 1] = 255 * (S2.IntersectionDistance(&R));
            rgb[(i * 3) + 2] = 0;
        }

        /*if (S.Intersects(&R)) {
            rgb[i * 3] = 255;
            rgb[(i * 3) + 1] = 0;
            rgb[(i * 3) + 2] = 0;
        }*/
    }

    stbi_write_png("image.png", 20, 20, 3, rgb, 0);
}

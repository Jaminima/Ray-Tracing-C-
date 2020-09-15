#include "Rendering.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main(int argc, char** argv)
{

	SceneObject Objs[] = {
		SceneObject(Vec3(4, 2, -5), Vec3(0, 255, 0), 4),
		SceneObject(Vec3(-4, 3, 4), Vec3(255, 0, 0), 4),
		SceneObject(Vec3(0, -5, 2), Vec3(0, 0, 255), 4)
	};

	/*Mesh* M = new Mesh();

	M->Triangles.Add(new Triangle(Vec3(0, -3, 2), Vec3(3, 0, -5), Vec3(-3, 0, 4), Vec3(255, 0, 0)));
	M->Triangles.Add(new Triangle(Vec3(-10, -8, 0), Vec3(-10, 8, 0), Vec3(00, 0, 20), Vec3(0, 120, 0)));

	Objs.Add(M);*/
	
	unsigned char* rgb = RenderScene(Objs);

	stbi_write_png("image.png", 2 * ViewWidth / ViewSteps, 2 * ViewWidth / ViewSteps, 3, rgb, 0);
}
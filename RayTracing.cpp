#include "Window.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

Camera mainCamera;

bool pick_accelerator()
{
	std::vector<accelerator> accs = accelerator::get_all();
	accelerator chosen_one;

	auto result = std::find_if(accs.begin(), accs.end(),
		[](const accelerator& acc) {
			return !acc.is_emulated &&
				acc.supports_double_precision &&
				!acc.has_display;
		});

	if (result != accs.end()) {
		chosen_one = *(result);
	}

	std::wcout << chosen_one.description << std::endl;
	bool success = accelerator::set_default(chosen_one.device_path);
	return success;
}

int main(int argc, char** argv)
{
	pick_accelerator();

	//stbi_write_png("image.png", px, px, 4, rgb, 0);

	lights[0].Position = Vec3(0, -5, 2);
	//lights[0].colormul = Vec3(0.5f, 0.8f, 0.5f);

	lights[1].Position = Vec3(-5, 5, 1);
	//lights[1].colormul = Vec3(0.8f, 0.0f, 0.8f);

	Sphere s0 = Sphere();

	s0.Center = Vec3(5, -5, 0);
	s0.color = Color(0, 255, 0);

	sceneObjects[0] = s0;

	Sphere s1 = Sphere();

	s1.Center = Vec3(-3, 5, -4);
	s1.color = Color(255, 0, 0);

	sceneObjects[1] = s1;

	Sphere s2 = Sphere();

	s2.Center = Vec3(-5, 0, -10);
	s2.color = Color(0, 0, 255);

	sceneObjects[2] = s2;

	Sphere s3 = Sphere();

	s3.Center = Vec3(10, 0, -10);

	sceneObjects[3] = s3;

	Sphere s4 = Sphere();

	s4.color = Color(200, 200, 200);

	sceneObjects[4] = s4;

	SetupFrame(argc, argv);

	while (true) {}
}

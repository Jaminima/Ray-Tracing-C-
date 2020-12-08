#include "Window.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"

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

	rgb = (Color*)malloc(px * py * sizeof(Color));

	//stbi_write_png("image.png", px, px, 4, rgb, 0);

	spheres = new Sphere[totalSpheres];
	lights = new Light[totalLights];

	lights[0].Position = Vec3(-5, 0, 0);
	lights[0].colormul = Vec3(1, 1, 1);

	spheres[0].Center = Vec3(5, -5, 0);
	spheres[0].color = Color(0, 255, 0);

	spheres[1].Center = Vec3(5, 5, 5);
	spheres[1].color = Color(255, 0, 0);

	spheres[2].Center = Vec3(-5, 0, -10);
	spheres[2].color = Color(0, 0, 255);

	spheres[3].Center = Vec3(-2, 5, 2);

	SetupFrame(argc, argv);

	while (true) {}
}

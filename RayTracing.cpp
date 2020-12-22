#include "Window.h"

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

	lights[0].Position = Vec3(0, -2, -5);
	lights[0].colormul = Vec3(0.5f, 0.8f, 0.5f);

	lights[1].Position = Vec3(-5, 2, -5);
	lights[1].colormul = Vec3(0.8f, 0.0f, 0.8f);

	Sphere s0 = Sphere(3, Vec3(5, -5, 0), Color(0, 255, 0));

	sceneObjects[0].SetSphere(s0);

	Sphere s1 = Sphere(3, Vec3(-3, 5, -4), Color(255, 0, 0));

	sceneObjects[1].SetSphere(s1);

	Sphere s2 = Sphere(3, Vec3(-5, 0, -10), Color(0, 0, 255));

	sceneObjects[2].SetSphere(s2);

	Sphere s3 = Sphere(3, Vec3(10, 0, -10));

	sceneObjects[3].SetSphere(s3);

	Triangle s4 = Triangle(Vec3(0, 0, 1), Vec3(5, 5, 0), Vec3(10, 0, -1), Color(200, 200, 200));

	sceneObjects[4].SetTriangle(s4);

	SetupFrame(argc, argv);

	while (true) {}
}
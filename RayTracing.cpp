#include "Window.h"

bool pick_accelerator()
{
	std::vector<accelerator> accs = accelerator::get_all();
	accelerator chosen_one;

	auto result = std::find_if(accs.begin(), accs.end(),
		[](const accelerator& acc)
	{
		return !acc.is_emulated &&
			acc.supports_double_precision &&
			!acc.has_display;
	});

	if (result != accs.end())
	{
		chosen_one = *(result);
	}

	std::wcout << chosen_one.description << std::endl;
	bool success = accelerator::set_default(chosen_one.device_path);
	return success;
}

int main(int argc, char** argv)
{
	pick_accelerator();

	lights[0].Position = Vec3(1, 5, -5);
	lights[0].colormul = Vec3(0.5f, 0.8f, 0.5f);

	lights[1].Position = Vec3(-1, -5, -5);
	lights[1].colormul = Vec3(0.8f, 0.0f, 0.8f);

	Triangle* t = new Triangle[]{
			Triangle(Vec3(0, 0, 1), Vec3(5, 5, 0), Vec3(10, 0, -1), Color(255, 0, 255)),
			Triangle(Vec3(0, 0, 1), Vec3(5, 5, 0), Vec3(10, -5, -1), Color(0, 255, 0))
	};

	sceneObjects[0].ImportTriangles(t, 2, sceneTriangles);

	sceneObjects[0].OuterCollider = Sphere(15.0f, sceneTriangles[0].ApproxPosition());

	sceneObjects[0].color = Color(0, 0, 255);

	SetupFrame(argc, argv);

	while (true)
	{
	}
}
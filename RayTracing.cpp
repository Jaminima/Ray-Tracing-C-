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
		//Front
			Triangle(Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(0, 1, 1), Custom1),
			Triangle(Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 1), Custom1),

		//Back
			Triangle(Vec3(1, 0, 0), Vec3(1, 1, 0), Vec3(1, 1, 1), Custom1),
			Triangle(Vec3(1, 0, 0), Vec3(1, 0, 1), Vec3(1, 1, 1), Custom1),

		//Left
			Triangle(Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(1, 1, 0), Custom1),
			Triangle(Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(1, 1, 0), Custom1),

		//Right
			Triangle(Vec3(0, 0, 1), Vec3(0, 1, 1), Vec3(1, 1, 1), Custom1),
			Triangle(Vec3(0, 0, 1), Vec3(1, 0, 1), Vec3(1, 1, 1), Custom1),

		//Bottom
			Triangle(Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(1, 0, 1), Custom1),
			Triangle(Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(1, 0, 1), Custom1),

		//Top
			Triangle(Vec3(0, 1, 0), Vec3(1, 1, 0), Vec3(1, 1, 1), Custom1),
			Triangle(Vec3(0, 1, 0), Vec3(0, 1, 1), Vec3(1, 1, 1), Custom1),

	};

	for (unsigned int i = 0;i < totalSceneObjects;i++) {
		sceneObjects[i].ImportTriangles(t, 10, sceneTriangles);
		sceneObjects[i].OuterCollider = Sphere(15.0f, sceneObjects[i].ApproxPosition(sceneTriangles));
		sceneObjects[i].Position = Vec3(rand() % 40 - 20, rand() % 40 - 20, rand() % 40 - 20);
	}

	SetupFrame(argc, argv);

	while (true)
	{
	}
}
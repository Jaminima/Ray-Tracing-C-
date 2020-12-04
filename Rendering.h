#pragma once
//#include "List.h"
#include "Sphere.h"

#include "Const.h"

#include <amp.h>
using namespace concurrency;

Color* RenderScene() {
	Color* rgb = (Color*)malloc(px*py*sizeof(Color));

	array_view<Color, 2> ColorView(px, py, rgb);

	parallel_for_each(
		ColorView.extent,
		[=](index<2> idx) restrict(amp) {
			Sphere s = Sphere();
			ColorView[idx[0]][idx[1]] = s.RenderRay(idx);
		}
	);

	ColorView.synchronize();

	return rgb;
}
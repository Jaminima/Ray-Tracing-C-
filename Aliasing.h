#pragma once
#include "Rendering.h"
#include <amp_math.h>

const int fxaa_center = 9,
fxaa_adjacent = 2,
fxaa_corner = 1,
fxaa_edge = 1,
fxaa_sum = fxaa_center + (fxaa_adjacent * 4) + (fxaa_corner * 4) + (4 * fxaa_edge);

void FXAA(array_view<Color,2> rgbData) {
	float fxaa_div = 1.0f / fxaa_sum;

	parallel_for_each(
		rgbData.extent,
		[=](index<2>idx) restrict(amp) {
			Vec3 aaColor = rgbData[idx].GetRGB() * fxaa_center;

			Vec3 adjacentcolor = rgbData[idx[0]][idx[1] + 1].GetRGB();

			adjacentcolor += rgbData[idx[0]][idx[1] - 1].GetRGB();
			adjacentcolor += rgbData[idx[0] + 1][idx[1]].GetRGB();
			adjacentcolor += rgbData[idx[0] - 1][idx[1]].GetRGB();

			adjacentcolor = adjacentcolor * fxaa_adjacent;

			Vec3 cornercolor = rgbData[idx[0] + 1][idx[1] + 1].GetRGB();

			cornercolor += rgbData[idx[0] - 1][idx[1] + 1].GetRGB();
			cornercolor += rgbData[idx[0] + 1][idx[1] - 1].GetRGB();
			cornercolor += rgbData[idx[0] - 1][idx[1] - 1].GetRGB();

			cornercolor = cornercolor * fxaa_corner;

			Vec3 edgecolor = rgbData[idx[0] + 2][idx[1]].GetRGB();

			edgecolor += rgbData[idx[0] - 2][idx[1]].GetRGB();
			edgecolor += rgbData[idx[0]][idx[1] + 2].GetRGB();
			edgecolor += rgbData[idx[0]][idx[1] - 2].GetRGB();

			edgecolor = edgecolor * fxaa_edge;

			aaColor = (aaColor + adjacentcolor + cornercolor + edgecolor) * fxaa_div;

			rgbData[idx] = Color(aaColor.x, aaColor.y, aaColor.z);
		});
}

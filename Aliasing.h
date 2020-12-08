#pragma once
#include "Rendering.h"
#include <amp_math.h>

const int fxaa_center = 9,
fxaa_adjacent = 2,
fxaa_corner = 1,
fxaa_sum = fxaa_center + (fxaa_adjacent * 4) + (fxaa_corner * 4);


void FXAA(Color* rgb) {
	float fxaa_div = 1.0f / fxaa_sum;

	array_view<Color,2> rgbData(py, px, rgb);

	parallel_for_each(
		rgbData.extent,
		[=](index<2>idx) restrict(amp) {
			Vec3 aaColor(0, 0, 0);

			aaColor += rgbData[idx].GetRGB() * fxaa_center;

			aaColor += rgbData[idx[0]][idx[1]+1].GetRGB() * fxaa_adjacent;
			aaColor += rgbData[idx[0]][idx[1]-1].GetRGB() * fxaa_adjacent;
			aaColor += rgbData[idx[0]+1][idx[1]].GetRGB() * fxaa_adjacent;
			aaColor += rgbData[idx[0]-1][idx[1]].GetRGB() * fxaa_adjacent;

			aaColor += rgbData[idx[0] + 1][idx[1] + 1].GetRGB() * fxaa_corner;
			aaColor += rgbData[idx[0] - 1][idx[1] + 1].GetRGB() * fxaa_corner;
			aaColor += rgbData[idx[0] + 1][idx[1] - 1].GetRGB() * fxaa_corner;
			aaColor += rgbData[idx[0] - 1][idx[1] - 1].GetRGB() * fxaa_corner;

			aaColor = aaColor * fxaa_div;

			rgbData[idx] = Color(aaColor.x, aaColor.y, aaColor.z);
		});

	rgbData.synchronize();
}
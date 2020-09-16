#pragma once
#include "Rendering.h"
#include <amp_math.h>

using namespace concurrency::fast_math;

void FXAA(unsigned char* rgb) {
	array_view<unsigned int> rgbData(imageMemory/4, (unsigned int*)rgb);

	unsigned int PreRow = ViewWS * 3 / 4;

	parallel_for_each(
		rgbData.extent,
		[=](index<1>i) restrict(amp) {
			unsigned int res = 0;

			for (unsigned int k = 0; k < 4; k++) {
				unsigned int T = (rgbData[i]>>8*k) & 0xFF;

				T += (rgbData[i - PreRow] >> 8 * k) & 0xFF;
				T += (rgbData[i + PreRow] >> 8 * k) & 0xFF;

				T += (rgbData[i] >> 8 * (k+4)) & 0xFF;
				T += (rgbData[i] >> 8 * (k+4)) & 0xFF;

				T += (rgbData[i - PreRow] >> 8 * (k - 4)) & 0xFF;
				T += (rgbData[i + PreRow] >> 8 * (k - 4)) & 0xFF;

				T += (rgbData[i - PreRow] >> 8 * (k + 4)) & 0xFF;
				T += (rgbData[i + PreRow] >> 8 * (k + 4)) & 0xFF;

				T /= 9;

				res += T << (k * 8);
			}

			rgbData[i] = res;
		});

	rgbData.synchronize();

	//std::copy(rgbData.data(), rgbData.data() + imageMemory/4, rgb);
}
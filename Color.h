#pragma once
#include "vec3.h"

struct Color {
public:
	unsigned int rgba = 0x0F000000;

	void SetR(unsigned int r) restrict(amp, cpu) { rgba = rgba | ((r & 0xFF) << 16); }
	void SetG(unsigned int g) restrict(amp, cpu) { rgba = rgba | ((g & 0xFF) << 8); }
	void SetB(unsigned int b) restrict(amp, cpu) { rgba = rgba | ((b & 0xFF)); }

	unsigned int GetR()restrict(amp, cpu) { return (rgba >> 16) & 0xFF; }
	unsigned int GetG()restrict(amp, cpu) { return (rgba >> 8) & 0xFF; }
	unsigned int GetB()restrict(amp, cpu) { return rgba & 0xFF; }

	Vec3 GetRGB() restrict(amp, cpu) { return Vec3(GetR(), GetG(), GetB()); }

	Color(unsigned int r, unsigned int g, unsigned int b) restrict(amp, cpu) {
		rgba |= ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));
	}

	Color operator+(Color c) restrict(amp, cpu) {
		unsigned int r1 = (rgba >> 16) & 0xFF,
			g1 = (rgba >> 8) & 0xFF,
			b1 = rgba & 0xFF;

		unsigned int r2 = (c.rgba >> 16) & 0xFF,
			g2 = (c.rgba >> 8) & 0xFF,
			b2 = c.rgba & 0xFF;

		return Color(r1 + r2, g1 + g2, b1 + b2);
	}

	Color operator * (float f) restrict(amp, cpu) {
		unsigned int r = (rgba >> 16) & 0xFF,
			g = (rgba >> 8) & 0xFF,
			b = rgba & 0xFF;

		return Color(r * f, g * f, b * f);
	}

	Color operator * (Vec3 f) restrict(amp, cpu) {
		unsigned int r = (rgba >> 16) & 0xFF,
			g = (rgba >> 8) & 0xFF,
			b = rgba & 0xFF;

		return Color(r * f.z, g * f.y, b * f.x);
	}
};

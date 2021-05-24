#pragma once
#include "vec3.h"

struct Color
{
public:
	unsigned int rgba = 0x0F000000;

	void SetR(unsigned int r) restrict(amp, cpu) { rgba = rgba | ((r & 0xFF)); }
	void SetG(unsigned int g) restrict(amp, cpu) { rgba = rgba | ((g & 0xFF) << 8); }
	void SetB(unsigned int b) restrict(amp, cpu) { rgba = rgba | ((b & 0xFF) << 16); }

	unsigned int GetR()restrict(amp, cpu) { return (rgba) & 0xFF; }
	unsigned int GetG()restrict(amp, cpu) { return (rgba >> 8) & 0xFF; }
	unsigned int GetB()restrict(amp, cpu) { return (rgba >> 16) & 0xFF; }

	Vec3 GetRGB() restrict(amp, cpu) { return Vec3(GetR(), GetG(), GetB()); }

	Color(unsigned int r, unsigned int g, unsigned int b) restrict(amp, cpu)
	{
		rgba |= ((b & 0xFF) << 16) | ((g & 0xFF) << 8) | ((r & 0xFF));
	}

	Color operator+(Color c) restrict(amp, cpu)
	{
		unsigned int b1 = (rgba >> 16) & 0xFF,
			g1 = (rgba >> 8) & 0xFF,
			r1 = rgba & 0xFF;

		unsigned int b2 = (c.rgba >> 16) & 0xFF,
			g2 = (c.rgba >> 8) & 0xFF,
			r2 = c.rgba & 0xFF;

		return Color(r1 + r2, g1 + g2, b1 + b2);
	}

	Color operator *(float f) restrict(amp, cpu)
	{
		unsigned int b = (rgba >> 16) & 0xFF,
			g = (rgba >> 8) & 0xFF,
			r = rgba & 0xFF;

		return Color(r * f, g * f, b * f);
	}

	Color operator *(Vec3 f) restrict(amp, cpu)
	{
		unsigned int b = (rgba >> 16) & 0xFF,
			g = (rgba >> 8) & 0xFF,
			r = rgba & 0xFF;

		return Color(r * f.x, g * f.y, b * f.z);
	}
};

#ifndef __Colors
#define __Colors

Color Custom1(50,50,50);

Color White(255, 255, 255);
Color Black(0, 0, 0);

Color Red(255, 0, 0);
Color Green(0, 255, 0);
Color Blue(0, 0, 255);

#endif // !Colors

#pragma once

struct Color {
public:
	unsigned int rgba = 0xFF000000;

	void SetR(unsigned int r) restrict(amp) { rgba = rgba | ((r & 0xFF) << 16); }
	void SetG(unsigned int g) restrict(amp) { rgba = rgba | ((g & 0xFF) << 8); }
	void SetB(unsigned int b) restrict(amp) { rgba = rgba | ((b & 0xFF)); }

	Color(unsigned int r, unsigned int g, unsigned int b) {
		rgba |= ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));
	}

	Color(unsigned int r, unsigned int g, unsigned int b) restrict(amp) {
		rgba |= ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | ((b & 0xFF));
	}

	Color operator+(Color c) restrict(amp) {
		unsigned int r1 = (rgba >> 16) & 0xFF,
			g1 = (rgba >> 8) & 0xFF,
			b1 = rgba & 0xFF;

		unsigned int r2 = (c.rgba >> 16) & 0xFF,
			g2 = (c.rgba >> 8) & 0xFF,
			b2 = c.rgba & 0xFF;

		return Color(r1 + r2, g1 + g2, b1 + b2);
	}

	Color operator * (float f) restrict(amp) {
		unsigned int r = (rgba >> 16) & 0xFF,
			g = (rgba >> 8) & 0xFF,
			b = rgba & 0xFF;

		return Color(r * f, g * f, b * f);
	}
};

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

	void operator+=(Color c) restrict(amp) {
		rgba |= c.rgba;
	}
};
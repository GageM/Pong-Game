#pragma once

#include <stdint.h>

struct Color
{	
	uint8_t r, g, b, a;

	inline Color(const uint8_t& r_, const uint8_t& g_, const uint8_t& b_, const uint8_t& a_) : r(r_), g(g_), b(b_), a(a_) {}

	inline void Set(const uint8_t& r_, const uint8_t& g_, const uint8_t& b_, const uint8_t& a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	// Helpful default colors
	inline static Color Black() { return Color(0, 0, 0, 255); }
	inline static Color White() { return Color(255, 255, 255, 255); }

	inline static Color Red() { return Color(255, 0, 0, 255); }
	inline static Color Green() { return Color(0, 255, 0, 255); }
	inline static Color Blue() { return Color(0, 0, 255, 255); }

	inline static Color Yellow() { return Color(255, 255, 0, 255); }
	inline static Color Cyan() { return Color(0, 255, 255, 255); }
	inline static Color Magenta() { return Color(255, 0, 255, 255); }

	// The default colors of the commodore 64 because why not
	inline static Color C64Red() { return Color(136, 0, 0, 255); }
	inline static Color C64Cyan() { return Color(170, 255, 238, 255); }
	inline static Color C64Violet() { return Color(204, 68, 204, 255); }
	inline static Color C64Green() { return Color(0, 204, 85, 255); }
	inline static Color C64Blue() { return Color(136, 0, 0, 255); }
	inline static Color C64Yellow() { return Color(238, 238, 119, 255); }
	inline static Color C64Orange() { return Color(221, 136, 85, 255); }
	inline static Color C64Brown() { return Color(102, 68, 0, 255); }
	inline static Color C64LightRed() { return Color(255, 119, 119, 255); }
	inline static Color C64DarkGrey() { return Color(51, 51, 51, 255); }
	inline static Color C64Grey() { return Color(119, 119, 119, 255); }
	inline static Color C64LightGreen() { return Color(170, 255, 102, 255); }
	inline static Color C64LightBlue() { return Color(0, 136, 255, 255); }
	inline static Color C64LightGrey() { return Color(187, 187, 187, 255); }
};


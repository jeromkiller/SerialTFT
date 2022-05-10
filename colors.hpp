#pragma once

#include <stdint.h>

//basic colors values copied from prenticedavid
//https://github.com/prenticedavid/Adafruit_GC9102_kbv/blob/master/Adafruit_GC9102_kbv.h
namespace basicColors
{
	constexpr uint16_t BLACK = 0x0000;
	constexpr uint16_t NAVY = 0x000F;
	constexpr uint16_t DARKGREEN = 0x03E0;
	constexpr uint16_t DARKCYAN = 0x03EF;
	constexpr uint16_t MAROON = 0x7800;
	constexpr uint16_t PURPLE = 0x780F;
	constexpr uint16_t OLIVE = 0x7BE0;
	constexpr uint16_t LIGHTGREY = 0xC618;
	constexpr uint16_t DARKGREY = 0x7BEF;
	constexpr uint16_t BLUE = 0x001F;
	constexpr uint16_t GREEN = 0x07E0;
	constexpr uint16_t CYAN = 0x07FF;
	constexpr uint16_t RED = 0xF800;
	constexpr uint16_t MAGENTA = 0xF81F;
	constexpr uint16_t YELLOW = 0xFFE0;
	constexpr uint16_t WHITE = 0xFFFF;
	constexpr uint16_t ORANGE = 0xFD20;
	constexpr uint16_t GREENYELLOW = 0xAFE5;
	constexpr uint16_t PINK = 0xFC18;
}

class packedColor
{
public:
	union color_t
	{
		uint16_t rawColor;
		struct 
		{
			uint16_t blue : 5;
			uint16_t green : 6;
			uint16_t red : 5;
		};
	};

	packedColor();
	packedColor(uint16_t col);
	packedColor(uint8_t R, uint8_t G, uint8_t B);

	//induvidual colors
	uint8_t getRed() const;
	uint8_t getGreen() const;
	uint8_t getBlue() const;
	void setRed(const uint8_t newRed);
	void setGreen(const uint8_t newGreen);
	void setBlue(const uint8_t newBlue);

	//operator overloads
	inline packedColor& operator=(const uint16_t newColors) {setColors(newColors);	return *this;};
	inline operator uint16_t() const { return getColors(); };

	uint16_t  getColors() const;
	void setColors(const uint16_t newColors);

private:
	color_t m_packedColors;
};
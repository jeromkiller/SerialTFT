#pragma once

#include "colors.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

class Adafruit_GFX;
class serialPacketBase;
class paintPacketBase;
class paintTextPacket;

//this class is responsible for drawing the graphics to the tft displays
class serialPainter
{
	//how am i going to save the lists of displays...
	//probably as a vector, then you can use itterators for arrays of different sizes
public:
	serialPainter(Adafruit_GFX** displays, const uint8_t num_displays);
	~serialPainter();

	Adafruit_GFX& getDisplay(const uint8_t id);

	bool performCommand(const serialPacketBase& packet);

private:
	bool setPaintBaseParameters(const paintPacketBase& packet);
	bool setTextBaseParameters(const paintTextPacket& packet);

	Adafruit_GFX** m_displays;

	//currently supports up to 8 screens
	uint8_t m_activeScreens;
	uint8_t m_numDisplays;

	packedColor m_penColor;
	packedColor m_bgColor;
	uint16_t m_penX;
	uint16_t m_penY;
};

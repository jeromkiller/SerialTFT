#pragma once

#include "colors.hpp"
#include "serialPainterTypes.hpp"

#include <stdint.h>
#include <vector>
#include <memory>

#include <Adafruit_ImageReader.h>

class Adafruit_SPITFT;
class SdFat;
class serialPacketBase;
class paintPacketBase;
class paintPrimitivePacket;
class paintTextPacket;
class paintSavedBMPPacket;

//this class is responsible for drawing the graphics to the tft displays
class serialPainter
{
	//how am i going to save the lists of displays...
	//probably as a vector, then you can use itterators for arrays of different sizes
public:
	serialPainter(Adafruit_SPITFT** displays, const uint8_t num_displays, SdFat* sd);
	~serialPainter();

	Adafruit_SPITFT& getDisplay(const uint8_t id);

	bool performCommand(const serialPacketBase& packet);
	//prints a message to the first screen, usefull for debugging or errors
	void printMessage(const char* msg);

private:
	void setPaintBaseParameters(const paintPacketBase& packet);
	void setPaintPrimitiveParameters(const paintPrimitivePacket& packet);
	void setTextParameters(const paintTextPacket& packet);
	void setBMPParameters(const paintSavedBMPPacket& packet);
	void postDrawCommand();

	Adafruit_SPITFT** m_displays;
	SdFat* m_sd;
	Adafruit_ImageReader m_reader;

	//currently supports up to 8 screens
	uint8_t m_activeScreens;
	uint8_t m_numDisplays;
	paintingTypes::MultiscreenStyles m_screenStyle;	//todo: find a better name

	packedColor m_penColor;
	packedColor m_textColor;
	packedColor m_textBgColor;
	packedColor m_bmpTransColor;
	packedColor m_bmpBgColor;

	uint16_t m_penX;
	uint16_t m_penY;
};

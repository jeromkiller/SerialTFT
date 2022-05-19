#pragma once
//the big o'l handler that manages all this shit
#include "serialPainter.hpp"
#include "displaySerialBuffer.hpp"

#include <SdFat.h>

class Adafruit_SPITFT;
class serialPacketBase;
class macroPacket;

namespace mainObjectConstants
{
	constexpr uint8_t maxMacroRecursionDepth = 5;
	constexpr size_t maxPacketSize();
};

class serialTFTHandler
{
public:
	serialTFTHandler(Adafruit_SPITFT** displays, const uint8_t num_displays, SdFat* sd);
	~serialTFTHandler();

	//add a byte from the serial line, returns true if a command is finnished
	bool handleByte(const uint8_t inByte);

	//handle a packet, can be called publicly for testing
	void handlePacket(const serialPacketBase& pack);

	//reset the buffer, usefull for timeouts if something went wrong
	void resetBuffer();
private:

	void deserializeBuffer(displaySerialBuffer& buff);
	
	File m_macroFile;
	displaySerialBuffer m_inBuffer;
	serialPainter m_painter;
	SdFat& m_sd;
	serialPacketBase* const m_packPool;

protected:
	uint8_t m_macroRecursionDepth;
};

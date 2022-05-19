#include "deserializationHandler.hpp"

#include "serialPacketBaseTypes.hpp"
#include "controlSerialPacket.hpp"
#include "macroPacket.hpp"
#include "paintPrimitivePacket.hpp"
#include "paintTextPacket.hpp"
#include "paintSavedBMPPacket.hpp"
#include <string.h>

constexpr size_t mainObjectConstants::maxPacketSize()
{
	union allPackets
	{
		controlSerialPacket t1;
		macroPacket t2;
		paintPrimitivePacket t3;
		paintTextPacket t4;
		paintSavedBMPPacket t5;
	};

	return sizeof(allPackets);
}

serialTFTHandler::serialTFTHandler(Adafruit_SPITFT **displays, const uint8_t num_displays, SdFat *sd)
	: m_inBuffer(displaySerialBuffer())
	, m_painter(serialPainter(displays, num_displays, sd))
	, m_sd(*sd)
	, m_packPool(static_cast<serialPacketBase*>(malloc(mainObjectConstants::maxPacketSize())))
{
	m_macroRecursionDepth = 0;
}

serialTFTHandler::~serialTFTHandler()
{
	free(m_packPool);

	if(m_macroFile)
	{
		m_macroFile.close();
	}
}

bool serialTFTHandler::handleByte(const uint8_t inByte)
{
	m_inBuffer.add(inByte);
	if (!m_inBuffer.validatePacket())
	{
		return false;
	}

	deserializeBuffer(m_inBuffer);
	handlePacket(*m_packPool);
	m_inBuffer.clear();

	return true;
}

void serialTFTHandler::deserializeBuffer(displaySerialBuffer &buff)
{
	using pt = serialPacketBaseTypes::packetType;
	pt packType = buff.getPacketType();

	switch (packType)
		{
		case pt::CONTROL_PACKET:
		{
			new(m_packPool)controlSerialPacket();
			break;
		}
		case pt::MACRO_PACKET:
		{
			new(m_packPool)macroPacket();
			break;
		}
		case pt::SHAPE_PACKET:
		{
			new(m_packPool)paintPrimitivePacket();
			break;
		}
		case pt::TEXT_PACKET:
		{
			new(m_packPool)paintTextPacket();
			break;
		}
		case pt::BITMAP_PACKET:
		{
			new(m_packPool)paintSavedBMPPacket();
			break;
		}
		default:
		{
			//todo return some errorcode
			m_painter.printMessage("error: unknown packet type");
			return;
		}
	}

	if(!m_packPool->deserialize(buff))
	{
		m_painter.printMessage("error: deserializing packet failed");
	}
}

void serialTFTHandler::handlePacket(const serialPacketBase &pack)
{
	if(pack.getPacketType() == serialPacketBaseTypes::packetType::MACRO_PACKET)
	{
		if(m_macroRecursionDepth >= mainObjectConstants::maxMacroRecursionDepth)
		{
			m_painter.printMessage("error: max recursion depth 5");
			return;
		}
		m_macroRecursionDepth++;

		//if we already have a file open, close it so we can open the new file
		bool filePrevOpen = false;
		const uint8_t maxNameLen = 20;
		char prevName[maxNameLen];
		unsigned long prevFilePos;
		if(m_macroFile)
		{
			filePrevOpen = true;
			strncpy(prevName, m_macroFile.name(), maxNameLen);
			prevFilePos = m_macroFile.position();
			m_macroFile.close();
		}
	
		const macroPacket& macroPack = static_cast<const macroPacket&>(pack);
		//open the new file
		m_macroFile = m_sd.open(macroPack.macroPath, FILE_READ);
		if(!m_macroFile)
		{
			m_painter.printMessage("error: could not open file");
			//todo: throw some error
			m_macroRecursionDepth--;
			return;
		}

		//loop through the file performing each command
		while(m_macroFile.available())
		{
			m_inBuffer.clear();
			//get the length
			uint16_t len = 0;
			m_macroFile.read(&len, sizeof(len));
			
			m_inBuffer.add(len);
			//get the rest of the packet
			for(uint16_t i = sizeof(len); i < len; i++)
			{
				const uint8_t b = m_macroFile.read();
				m_inBuffer.add(b);
			}
			//deserialize, and handle
			deserializeBuffer(m_inBuffer);
			handlePacket(*m_packPool);
		}
		
		//this macro file is done, 
		m_macroFile.close();

		//restore the previous one if we had one
		if(filePrevOpen)
		{
			m_macroFile = m_sd.open(prevName, FILE_READ);
			m_macroFile.seek(prevFilePos);
		}

		m_macroRecursionDepth--;
	}
	else
	{
		m_painter.performCommand(pack);
	}
}

void serialTFTHandler::resetBuffer()
{
	m_inBuffer.clear();
}

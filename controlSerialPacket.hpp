#pragma once
#include "serialPacketBase.hpp"
#include "packetProperty.hpp"
#include <vector>
#include <stdint.h>

class controlSerialPacket : 
	public serialPacketBase
{
public:
	controlSerialPacket();

	virtual bool serialize(displaySerialBuffer& buffer) final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//data
	packProperty<uint8_t> screenSelect;
	packProperty<uint8_t> screenBrightness;
	packProperty<uint8_t> keyBrightness;
	packProperty<uint8_t> m_screenOrient;
};

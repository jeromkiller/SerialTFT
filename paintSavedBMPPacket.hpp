#pragma once

#include "paintPacketBase.hpp"
#include "packetProperty.hpp"
#include "colors.hpp"

#include <stdint.h>

class paintSavedBMPPacket :
	public paintPacketBase
{
public:
	paintSavedBMPPacket();

	virtual bool serialize(displaySerialBuffer& buffer) final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//data
	packProperty<char*> BMPPath;
	packProperty<packedColor> bgColor;
	packProperty<packedColor> transColor;
};

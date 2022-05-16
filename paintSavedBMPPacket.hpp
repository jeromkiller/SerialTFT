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

	virtual bool serialize(displaySerialBuffer& buffer) const final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//data
	packProperty<const char*> BMPPath;
	packProperty<packedColor> bgColor;
	packProperty<packedColor> transColor;
};

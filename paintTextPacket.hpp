#pragma once

#include "paintPacketBase.hpp"
#include "packetProperty.hpp"
#include "colors.hpp"
#include <stdint.h>

class paintTextPacket :
	public paintPacketBase
{
public:
	paintTextPacket();

	virtual bool serialize(displaySerialBuffer& buffer) final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//data
	packProperty<uint8_t> textSize;
	packProperty<bool> useWrapping;
	packProperty<char*> textPtr;
};
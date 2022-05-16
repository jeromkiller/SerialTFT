#pragma once

#include "serialPacketBase.hpp"
#include "packetProperty.hpp"
#include <stdint.h>

class displaySerialBuffer;

class macroPacket :
	public serialPacketBase
{
public:
	macroPacket();

	virtual bool serialize(displaySerialBuffer& buffer) const final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//data
	packProperty<const char*> macroPath;
};

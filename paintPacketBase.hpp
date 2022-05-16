#pragma once

#include "serialPacketBase.hpp"
#include "packetProperty.hpp"
#include <stdint.h>

class displaySerialBuffer;

class paintPacketBase : 
	public serialPacketBase
{
public:
	virtual bool serialize(displaySerialBuffer& buffer) const override;
	virtual bool deserialize(displaySerialBuffer& buffer) override;

	//data
	packProperty<uint16_t> cursorX;
	packProperty<uint16_t> cursorY;

protected:
	//this class cannot be created by the user
	paintPacketBase(serialPacketBaseTypes::packetType pType);
};

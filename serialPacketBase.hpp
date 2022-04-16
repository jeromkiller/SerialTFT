#pragma once
#include "serialPacketBaseTypes.hpp"
#include "packetFlags.hpp"
#include <vector>
#include <stdint.h>

class displaySerialBuffer;

class serialPacketBase
{
public:
	serialPacketBaseTypes::packetType getPacketType() const;

	virtual bool serialize(displaySerialBuffer& buffer);
	virtual bool deserialize(displaySerialBuffer& buffer);

protected:
	//this class cannot be created by the user
	serialPacketBase(const serialPacketBaseTypes::packetType pType);
	bool finalizePacket(displaySerialBuffer& buffer);
	packetFlags m_flags;

private:
	const serialPacketBaseTypes::packetType m_pType;
};
#include "serialPacketBase.hpp"
#include "displaySerialBuffer.hpp"
#include <assert.h>

using namespace serialPacketBaseTypes;

serialPacketBaseTypes::packetType serialPacketBase::getPacketType() const
{
	return m_pType;
}

serialPacketBase::serialPacketBase(const packetType pType) : m_pType(pType)
{
	m_flags.rawByte = 0;
}

void serialPacketBase::setRawFlags(const dataFlags newFlags)
{
	m_flags = newFlags;
}

void serialPacketBase::setRawFlags(const uint8_t newFlags)
{
	m_flags.rawByte = newFlags;
}

serialPacketBase::dataFlags serialPacketBase::getFlags() const
{
	return m_flags;
}

uint8_t serialPacketBase::getRawFlags() const
{
	return m_flags.rawByte;
}

bool serialPacketBase::serialize(displaySerialBuffer& buffer)
{
	if(buffer.getPacketLength() > 0)
	{
		return false;
	}
	//add a byte for the packet size, to be determined later
	buffer.add((uint8_t)0);

	//add the packet type 
	buffer.add(m_pType);
	buffer.add(m_flags);
	return true;
}

bool serialPacketBase::finalizePacket(displaySerialBuffer& buffer)
{
	//add the packet length to the packet + space for the packet length byte, and the 
	const uint8_t packetSize = buffer.getBufferSize() + 1;
	std::vector<uint8_t>& rawBuffer = buffer.getBuffer();
	rawBuffer[packetIndex::PACKET_LENGTH] = packetSize;

	//calculate and add the crc byte
	//add an empty byte to finalize the packet and to calculate the crc
	rawBuffer.push_back(0);
	*rawBuffer.end() = buffer.calcCRCbyte();

	return true;
}

bool serialPacketBase::deserialize(displaySerialBuffer& buffer)
{
	//validate if we got the full package
	if(!buffer.validatePacket())
	{
		return false;
	}

	buffer.get(&m_pType, packetIndex::PACKET_TYPE);
	buffer.get(&m_flags, packetIndex::PACKET_FLAGS);

	return true;
}
#include "displaySerialBuffer.hpp"

#include <stdlib.h>
#include <assert.h>
#include <cstring>

displaySerialBuffer::displaySerialBuffer()
{
	m_buffer.clear();
	m_dataIndex = 0;
}

std::vector<uint8_t>& displaySerialBuffer::getBuffer()
{
	return m_buffer;
}

void displaySerialBuffer::setPacketIndex(uint16_t newIndex)
{
	m_dataIndex = newIndex;
}

uint16_t displaySerialBuffer::getPacketLength() const
{
	if(m_buffer.empty())
	{
		return 0;
	}
	
	return *m_buffer.cbegin();
}

uint16_t displaySerialBuffer::getBufferSize() const
{
	return m_buffer.size();
}

serialPacketBaseTypes::packetType displaySerialBuffer::getPacketType() const
{
	using pt = serialPacketBaseTypes::packetType;
	using pi = serialPacketBaseTypes::packetIndex;
	if(m_buffer.size() < 2)
	{
		return pt::UNKNOWN_PACKET;
	}

	return static_cast<pt>(m_buffer[pi::PACKET_TYPE]);
}

bool displaySerialBuffer::fullPacketRecieved() const
{
	return getPacketLength() == m_buffer.size();
}

uint8_t displaySerialBuffer::calcCRCbyte()
{
	uint8_t crcByte = 0;
	for(auto byte = m_buffer.begin(); byte != (m_buffer.end()-1); byte++)
	{
		crcByte ^= *byte;
	}
	return crcByte;
}

bool displaySerialBuffer::validatePacket()
{
	if(m_buffer.empty())
	{
		return false;
	}

	bool ret = true;
	ret &= fullPacketRecieved();
	ret &= calcCRCbyte() == *(m_buffer.end()-1);

	return ret;
}

void displaySerialBuffer::clear()
{
	m_buffer.clear();
}

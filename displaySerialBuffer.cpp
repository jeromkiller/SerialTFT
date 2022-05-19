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

void displaySerialBuffer::setPacketIndex(uint8_t newIndex)
{
	m_dataIndex = newIndex;
}

uint8_t displaySerialBuffer::getPacketLength() const
{
	if(m_buffer.empty())
	{
		return 0;
	}
	
	return *m_buffer.cbegin();
}

uint8_t displaySerialBuffer::getBufferSize() const
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
	//don't even bother checking
	if(m_buffer.empty())
	{
		return false;
	}

	//check if we have the entire packet
	if(!fullPacketRecieved())
	{
		return false;
	}

	//check if we got the packet correctly
	if(calcCRCbyte() != *(m_buffer.end()-1))
	{
		//if the packet is wrong, then reset the buffer
		//hopefully the next packet does work
		clear();
		return false;
	}

	//all good
	return true;
}

void displaySerialBuffer::clear()
{
	m_buffer.clear();
}

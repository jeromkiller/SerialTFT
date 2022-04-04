#include "paintTextPacket.hpp"
#include "displaySerialBuffer.hpp"

paintTextPacket::paintTextPacket() :
	paintPacketBase(serialPacketBaseTypes::packetType::TEXT_PACKET)
{
	m_textPtr = nullptr;
};

bool paintTextPacket::serialize(displaySerialBuffer& buffer)
{
	if(!paintPacketBase::serialize(buffer))
	{
		return false;
	}

	if(getWriteTextFlag())
	{
		//when serializing, copy the string to the send buffer
		buffer.add_buffer(m_textPtr, strlen(m_textPtr) + 1);	//+1 to keep the null byte too
	}

	return true;
}

bool paintTextPacket::deserialize(displaySerialBuffer& buffer)
{
	bool ret = paintPacketBase::deserialize(buffer);

	if(!ret)
	{
		return false;
	}

	if(getWriteTextFlag())
	{
		uint8_t dummy;
		buffer.get_buffer_ptr(&m_textPtr, &dummy);
	}

	return true;
}

//high nibble flag control
bool paintTextPacket::getWriteTextFlag() const
{
	return m_flags.flag0;
}

void paintTextPacket::setWriteTextFlag(const bool flag)
{
	m_flags.flag0 = flag;
}

char* paintTextPacket::getTextPtr() const
{
	return m_textPtr;
}

void paintTextPacket::setTextPtr(char* textPtr)
{
	setWriteTextFlag(true);
	m_textPtr = textPtr;
}

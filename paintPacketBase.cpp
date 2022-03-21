#include "paintPacketBase.hpp"
#include "displaySerialBuffer.hpp"

paintPacketBase::paintPacketBase(serialPacketBaseTypes::packetType pType) :
	serialPacketBase(pType)
{
	m_cursorX = 0;
	m_cursorY = 0;
}

bool paintPacketBase::serialize(displaySerialBuffer& buffer)
{
	if(!serialPacketBase::serialize(buffer))
	{
		return false;
	}

	if(getCursorXFlag())
	{
		buffer.add(m_cursorX);
	}
	if(getCursorYFlag())
	{
		buffer.add(m_cursorY);
	}
	if(getPenColorFlag())
	{
		buffer.add(m_penColor);
	}
	if(getBgColorFlag())
	{
		buffer.add(m_bgColor);
	}

	return true;
}

bool paintPacketBase::deserialize(displaySerialBuffer& buffer)
{
	using namespace serialPacketBaseTypes;

	bool ret = serialPacketBase::deserialize(buffer);

	if(!ret)
	{
		return false;
	}

	//start reading the data
	buffer.setPacketIndex(packetIndex::PACKET_DATA);

	if(getCursorXFlag())
	{
		buffer.get(&m_cursorX);
	}
	if(getCursorYFlag())
	{
		buffer.get(&m_cursorY);
	}
	if(getPenColorFlag())
	{
		buffer.get(&m_penColor);
	}
	if(getBgColorFlag())
	{
		buffer.get(&m_bgColor);
	}

	return ret;
}

//high nibble flag control
bool paintPacketBase::getCursorXFlag() const
{
	return m_flags.flag4;
}

bool paintPacketBase::getCursorYFlag() const
{
	return m_flags.flag5;
}

bool paintPacketBase::getPenColorFlag() const
{
	return m_flags.flag6;
}

bool paintPacketBase::getBgColorFlag() const
{
	return m_flags.flag7;
}

void paintPacketBase::setCursorXFlag(const bool flag)
{
	m_flags.flag4 = flag;
}

void paintPacketBase::setCursorYFlag(const bool flag)
{
	m_flags.flag5 = flag;
}

void paintPacketBase::setPenColorFlag(const bool flag)
{
	m_flags.flag6 = flag;
}

void paintPacketBase::setBgColorFlag(const bool flag)
{
	m_flags.flag7 = flag;
}

//data getters and setters
uint16_t paintPacketBase::getCursorX() const
{
	return m_cursorX;
}

uint16_t paintPacketBase::getCursorY() const
{
	return m_cursorY;
}

packedColor paintPacketBase::getPenColor() const
{
	return m_penColor;
}

packedColor paintPacketBase::getBgColor() const
{
	return m_bgColor;
}

void paintPacketBase::setCursorX(const uint16_t newCursorX)
{
	m_cursorX = newCursorX;
	setCursorXFlag(true);
}

void paintPacketBase::setCursorY(const uint16_t newCursorY)
{
	m_cursorY = newCursorY;
	setCursorYFlag(true);
}

void paintPacketBase::setPenColor(const packedColor newColor)
{
	m_penColor = newColor;
	setPenColorFlag(true);
}

void paintPacketBase::setBgColor(const packedColor newColor)
{
	m_bgColor = newColor;
	setBgColorFlag(true);
}
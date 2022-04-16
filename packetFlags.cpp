#include "packetFlags.hpp"

packetFlags::packetFlags()
{
	m_flagByte = 0;
}

bool packetFlags::getFlag(const uint8_t id) const
{
	//I'm going to do the flags by hand, because bitfields aren't that portable
	return 0 != (m_flagByte & (1 << id));
}

void packetFlags::setFlag(const uint8_t id, const bool value)
{
	const uint8_t setBit = 1 << id;
	if(value)
	{
		m_flagByte |= setBit;
	}
	else
	{
		m_flagByte &= ~setBit;
	}
}

void packetFlags::setFlag(const uint8_t id)
{
	setFlag(id, true);
}
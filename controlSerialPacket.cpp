#include "controlSerialPacket.hpp"
#include "serialPacketBaseTypes.hpp"
#include "displaySerialBuffer.hpp"

controlSerialPacket::controlSerialPacket() :
	serialPacketBase(serialPacketBaseTypes::packetType::CONTROL_PACKET)
{
	m_flags.rawByte = 0;
	m_screenSelect = 0;
	m_screenBrightness = 0;
	m_keyBrightness = 0;
}

bool controlSerialPacket::serialize(displaySerialBuffer& buffer)
{
	if(!serialPacketBase::serialize(buffer))
	{
		return false;
	}

	if(getScreenSelectFlag())
	{
		buffer.add(m_screenSelect);
	}
	if(getScreenBrightnessFlag())
	{
		buffer.add(m_screenBrightness);
	}
	if(getKeyBrightnessFlag())
	{
		buffer.add(m_keyBrightness);
	}

	return finalizePacket(buffer);
}

bool controlSerialPacket::deserialize(displaySerialBuffer& buffer)
{
	using namespace serialPacketBaseTypes;

	bool ret = true;
	ret &= serialPacketBase::deserialize(buffer);

	if(!ret)
	{
		return false;
	}

	buffer.setPacketIndex(packetIndex::PACKET_DATA);

	if(getScreenSelectFlag())
	{
		buffer.get(&m_screenSelect);
	}
	if(getScreenBrightnessFlag())
	{
		buffer.get(&m_screenBrightness);
	}
	if(getKeyBrightnessFlag())
	{
		buffer.get(&m_keyBrightness);
	}

	return true;
}

bool controlSerialPacket::getScreenSelectFlag() const
{
	return m_flags.flag0;
}
bool controlSerialPacket::getScreenBrightnessFlag() const
{
	return m_flags.flag1;
}
bool controlSerialPacket::getKeyBrightnessFlag() const
{
	return m_flags.flag2;
}
void controlSerialPacket::setScreenSelectFlag(bool flag)
{
	m_flags.flag0 = flag;
}
void controlSerialPacket::setScreenBrightnessFlag(bool flag)
{
	m_flags.flag1 = flag;
}
void controlSerialPacket::setKeyBrightnessFlag(bool flag)
{
	m_flags.flag2 = flag;
}

uint8_t controlSerialPacket::getScreenSelect()const
{
	return m_screenSelect;
}

uint8_t controlSerialPacket::getScreenBrightness()const
{
	return m_screenBrightness;
}

uint8_t controlSerialPacket::getKeyBrightness()const
{
	return m_keyBrightness;
}

void controlSerialPacket::setScreenSelect(uint8_t newVal)
{
	m_screenSelect = newVal;
	setScreenSelectFlag(true);
}

void controlSerialPacket::setScreenBrightness(uint8_t newVal)
{
	m_screenBrightness = newVal;
	setScreenBrightnessFlag(true);
}

void controlSerialPacket::setKeyBrightness(uint8_t newVal)
{
	m_keyBrightness = newVal;
	setKeyBrightnessFlag(true);
}
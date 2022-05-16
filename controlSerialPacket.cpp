#include "controlSerialPacket.hpp"
#include "serialPacketBaseTypes.hpp"
#include "displaySerialBuffer.hpp"

controlSerialPacket::controlSerialPacket() 
	: serialPacketBase(serialPacketBaseTypes::packetType::CONTROL_PACKET)
	, screenSelect(packProperty<uint8_t>(m_flags, 0))
	, screenBrightness(packProperty<uint8_t>(m_flags, 1))
	, keyBrightness(packProperty<uint8_t>(m_flags, 2))
	, m_screenOrient(packProperty<uint8_t>(m_flags, 3))
	, m_multiStyle(packProperty<paintingTypes::MultiscreenStyles>(m_flags, 4))
{
}

bool controlSerialPacket::serialize(displaySerialBuffer& buffer) const
{
	bool ret = serialPacketBase::serialize(buffer);
	if(!ret)
	{
		return false;
	}

	ret &= screenSelect.serialize(buffer);
	ret &= screenBrightness.serialize(buffer);
	ret &= keyBrightness.serialize(buffer);
	ret &= m_screenOrient.serialize(buffer);
	ret &= m_multiStyle.serialize(buffer);

	ret &= finalizePacket(buffer);

	return ret;
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

	ret &= screenSelect.deserialize(buffer);
	ret &= screenBrightness.deserialize(buffer);
	ret &= keyBrightness.deserialize(buffer);
	ret &= m_screenOrient.deserialize(buffer);
	ret &= m_multiStyle.deserialize(buffer);

	return ret;
}

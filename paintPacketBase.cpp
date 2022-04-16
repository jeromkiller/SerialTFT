#include "paintPacketBase.hpp"
#include "displaySerialBuffer.hpp"

paintPacketBase::paintPacketBase(serialPacketBaseTypes::packetType pType) 
	: serialPacketBase(pType)
	, cursorX(packProperty<uint16_t>(m_flags, 4))
	, cursorY(packProperty<uint16_t>(m_flags, 5))
	, penColor(packProperty<packedColor>(m_flags, 6))
	, bgColor(packProperty<packedColor>(m_flags, 7))
{
}

bool paintPacketBase::serialize(displaySerialBuffer& buffer)
{
	bool ret = serialPacketBase::serialize(buffer);
	if(!ret)
	{
		return false;
	}

	cursorX.serialize(buffer);
	cursorY.serialize(buffer);
	penColor.serialize(buffer);
	bgColor.serialize(buffer);

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

	ret &= cursorX.deserialize(buffer);
	ret &= cursorY.deserialize(buffer);
	ret &= penColor.deserialize(buffer);
	ret &= bgColor.deserialize(buffer);

	return ret;
}

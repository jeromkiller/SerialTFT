#include "paintPacketBase.hpp"
#include "displaySerialBuffer.hpp"

paintPacketBase::paintPacketBase(serialPacketBaseTypes::packetType pType) 
	: serialPacketBase(pType)
	, cursorX(packProperty<uint16_t>(m_flags, 0))
	, cursorY(packProperty<uint16_t>(m_flags, 1))
{
}

bool paintPacketBase::serialize(displaySerialBuffer& buffer) const
{
	bool ret = serialPacketBase::serialize(buffer);
	if(!ret)
	{
		return false;
	}

	ret &= cursorX.serialize(buffer);
	ret &= cursorY.serialize(buffer);

	return ret;
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

	return ret;
}

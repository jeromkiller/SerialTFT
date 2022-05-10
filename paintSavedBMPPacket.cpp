#include "paintSavedBMPPacket.hpp"

paintSavedBMPPacket::paintSavedBMPPacket()
	: paintPacketBase(serialPacketBaseTypes::packetType::BITMAP_PACKET)
	, BMPPath(packProperty<const char*>(m_flags, 2))
	, bgColor(packProperty<packedColor>(m_flags, 3))
	, transColor(packProperty<packedColor>(m_flags, 4))
{
};

bool paintSavedBMPPacket::serialize(displaySerialBuffer& buffer)
{
	bool ret = paintPacketBase::serialize(buffer);
	if(!ret)
	{
		return false;
	}

	ret &= BMPPath.serialize(buffer, strlen(BMPPath.value()) + 1);	//+1 to include the null character signifying the end of the string
	ret &= bgColor.serialize(buffer);
	ret &= transColor.serialize(buffer);

	ret &= finalizePacket(buffer);

	return ret;
}

bool paintSavedBMPPacket::deserialize(displaySerialBuffer& buffer)
{
	bool ret = paintPacketBase::deserialize(buffer);
	if(!ret)
	{
		return false;
	}

	ret &= BMPPath.deserialize(buffer);
	ret &= bgColor.deserialize(buffer);
	ret &= transColor.deserialize(buffer);

	return ret;
}
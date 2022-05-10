#include "paintTextPacket.hpp"
#include "displaySerialBuffer.hpp"

paintTextPacket::paintTextPacket()
	: paintPacketBase(serialPacketBaseTypes::packetType::TEXT_PACKET)
	, textSize(packProperty<uint8_t>(m_flags, 2))
	, useWrapping(packProperty<bool>(m_flags, 3))
	, penColor(packProperty<packedColor>(m_flags, 4))
	, bgColor(packProperty<packedColor>(m_flags, 5))
	, textPtr(packProperty<const char*>(m_flags, 6))
{
};

bool paintTextPacket::serialize(displaySerialBuffer& buffer)
{
	bool ret = paintPacketBase::serialize(buffer);
	if(!ret)
	{
		return false;
	}

	ret &= textSize.serialize(buffer);
	ret &= useWrapping.serialize(buffer);
	ret &= textPtr.serialize(buffer, strlen(textPtr.value()) + 1);	//+1 to include the null character signifying the end of the string 
	ret &= penColor.serialize(buffer);
	ret &= bgColor.serialize(buffer);

	ret &= finalizePacket(buffer);

	return ret;
}

bool paintTextPacket::deserialize(displaySerialBuffer& buffer)
{
	bool ret = paintPacketBase::deserialize(buffer);

	if(!ret)
	{
		return false;
	}

	ret &= textSize.deserialize(buffer);
	ret &= useWrapping.deserialize(buffer);
	ret &= textPtr.deserialize(buffer);
	ret &= penColor.deserialize(buffer);
	ret &= bgColor.deserialize(buffer);

	return ret;
}

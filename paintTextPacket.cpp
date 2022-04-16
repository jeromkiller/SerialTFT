#include "paintTextPacket.hpp"
#include "displaySerialBuffer.hpp"

paintTextPacket::paintTextPacket()
	: paintPacketBase(serialPacketBaseTypes::packetType::TEXT_PACKET)
	, textSize(packProperty<uint8_t>(m_flags, 0))
	, useWrapping(packProperty<bool>(m_flags, 1))
	, textPtr(packProperty<char*>(m_flags, 2))
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
	ret &= textPtr.serialize(buffer, strlen(textPtr.value()) + 1);	//+1 to include the null character signifying 

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

	return ret;
}

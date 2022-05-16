#include "macroPacket.hpp"

macroPacket::macroPacket()
	: serialPacketBase(serialPacketBaseTypes::packetType::MACRO_PACKET)
	, macroPath(packProperty<const char*>(m_flags, 1))
{	
};

bool macroPacket::serialize(displaySerialBuffer& buffer) const
{
	bool ret = serialPacketBase::serialize(buffer);
	if(!ret)
	{
		return false;
	}

	ret &= macroPath.serialize(buffer, strlen(macroPath.value()) + 1);	//+1 to include the null character for the end of the string

	ret &= finalizePacket(buffer);

	return ret;
}

bool macroPacket::deserialize(displaySerialBuffer& buffer)
{
	bool ret = serialPacketBase::deserialize(buffer);
	if(!ret)
	{
		return false;
	}

	ret &= macroPath.deserialize(buffer);

	return ret;
}

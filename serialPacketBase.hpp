#pragma once
#include "serialPacketBaseTypes.hpp"
#include <vector>
#include <stdint.h>

class displaySerialBuffer;

class serialPacketBase
{
public:
	union dataFlags
	{
		uint8_t rawByte;
		struct
		{
			union
			{
				struct
				{
					uint8_t LowNibble : 4;
					uint8_t highNibble : 4;
				};
				struct 
				{
					uint8_t flag0 : 1;
					uint8_t flag1 : 1;
					uint8_t flag2 : 1;
					uint8_t flag3 : 1;
					uint8_t flag4 : 1;
					uint8_t flag5 : 1;
					uint8_t flag6 : 1;
					uint8_t flag7 : 1;
				};	
			};
		};
	};

	serialPacketBaseTypes::packetType getPacketType() const;
	void setRawFlags(const dataFlags newFlags);
	void setRawFlags(const uint8_t newFlags);
	dataFlags getFlags() const;
	uint8_t getRawFlags() const;

	virtual bool serialize(displaySerialBuffer& buffer);
	virtual bool deserialize(displaySerialBuffer& buffer);

protected:
	//this class cannot be created by the user
	serialPacketBase(const serialPacketBaseTypes::packetType pType);
	bool finalizePacket(displaySerialBuffer& buffer);
	dataFlags m_flags;

private:
	const serialPacketBaseTypes::packetType m_pType;
};
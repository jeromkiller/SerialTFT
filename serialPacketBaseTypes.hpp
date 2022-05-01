#pragma once
#include <stdint.h>

namespace serialPacketBaseTypes
{
	enum class packetType : uint8_t
	{
		UNKNOWN_PACKET = 0,
		CONTROL_PACKET,
		MACRO_PACKET,
		SHAPE_PACKET,
		TEXT_PACKET,
		BITMAP_PACKET,
	};

	enum packetIndex : uint8_t
	{
		PACKET_LENGTH = 0,
		PACKET_TYPE = 1,
		PACKET_FLAGS = 2,
		PACKET_DATA = 3
	};
};

#pragma once
#include <inttypes.h>

class packetFlags
{
	public:	

	packetFlags();

	bool getFlag(const uint8_t id) const;
	void setFlag(const uint8_t id, bool value);
	void setFlag(const uint8_t id);
	
	uint8_t m_flagByte;
};
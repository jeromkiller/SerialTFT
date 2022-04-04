#pragma once

#include "paintPacketBase.hpp"
#include "colors.hpp"
#include <stdint.h>

class paintTextPacket :
	public paintPacketBase
{
public:
	paintTextPacket();

	virtual bool serialize(displaySerialBuffer& buffer) final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//high nibble flag control
	bool getWriteTextFlag() const;

	char* getTextPtr() const;
	void setTextPtr(char* textPtr);

private:
	void setWriteTextFlag(const bool flag);
	
	//data
	char* m_textPtr;
};
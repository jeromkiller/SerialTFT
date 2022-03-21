#pragma once

#include "serialPacketBase.hpp"
#include "colors.hpp"
#include <stdint.h>

class displaySerialBuffer;

class paintPacketBase : 
	public serialPacketBase
{
public:
	virtual bool serialize(displaySerialBuffer& buffer) override;
	virtual bool deserialize(displaySerialBuffer& buffer) override;

	//high nibble flag control
	bool getCursorXFlag() const;
	bool getCursorYFlag() const;
	bool getPenColorFlag() const;
	bool getBgColorFlag() const;

	//data getters and setters
	uint16_t getCursorX() const;
	uint16_t getCursorY() const;
	packedColor getPenColor() const;
	packedColor getBgColor() const;
	void setCursorX(const uint16_t newCursorX);
	void setCursorY(const uint16_t newCursorY);
	void setPenColor(const packedColor newColor);
	void setBgColor(const packedColor newColor);

protected:
	//this class cannot be created by the user
	paintPacketBase(serialPacketBaseTypes::packetType pType);

private:
	void setCursorXFlag(const bool flag); 
	void setCursorYFlag(const bool flag);
	void setPenColorFlag(const bool flag);
	void setBgColorFlag(const bool flag);

	//data
	uint16_t m_cursorX;
	uint16_t m_cursorY;
	packedColor m_penColor;
	packedColor m_bgColor;
};
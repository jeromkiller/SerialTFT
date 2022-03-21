#pragma once
#include "serialPacketBase.hpp"
#include <vector>
#include <stdint.h>

class controlSerialPacket : 
	public serialPacketBase
{
public:
	controlSerialPacket();

	virtual bool serialize(displaySerialBuffer& buffer) final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//flag control
	bool getScreenSelectFlag() const;
	bool getScreenBrightnessFlag() const;
	bool getKeyBrightnessFlag() const;

	//data getters and setters
	uint8_t getScreenSelect()const;
	uint8_t getScreenBrightness()const;
	uint8_t getKeyBrightness()const;
	void setScreenSelect(uint8_t newVal);
	void setScreenBrightness(uint8_t newVal);
	void setKeyBrightness(uint8_t newVal);

private:
	void setScreenSelectFlag(bool flag);
	void setScreenBrightnessFlag(bool flag);
	void setKeyBrightnessFlag(bool flag);

	//data
	uint8_t m_screenSelect;
	uint8_t m_screenBrightness;
	uint8_t m_keyBrightness;
};
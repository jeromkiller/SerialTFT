#pragma once

#include "paintPacketBase.hpp"
#include "packetProperty.hpp"
#include "colors.hpp"
#include <stdint.h>

class paintPrimitivePacket :
	public paintPacketBase
{
public:
	paintPrimitivePacket();

	//4 bits max, so max 16 options
	enum class primitives : uint8_t
	{
		none = 0,
		point,
		line,
		circle,
		filledCircle,
		rectangle,
		filledRectangle,
		roundedRectangle,
		filledRoundedRectangle,
		triangle,
		filledTriangle
	};

	virtual bool serialize(displaySerialBuffer& buffer) const final;
	virtual bool deserialize(displaySerialBuffer& buffer) final;

	//primitive is stored in the low nibble of the flags
	primitives getPrimitiveFlag() const;
	void setPrimitiveFlag(primitives primitive);

	//simple packet data
	packProperty<packedColor> penColor;

	//data getters and setters
	uint16_t getWidth() const;
	uint16_t getHeight() const;
	uint16_t getRadius() const;
	uint16_t getPointX1() const;
	uint16_t getPointY1() const;
	uint16_t getPointX2() const;
	uint16_t getPointY2() const;
	void setWidth(uint16_t newWidth);
	void setHeight(uint16_t newHeight);
	void setRadius(uint16_t newRadius);
	void setPointX1(uint16_t newTriangleX1);
	void setPointY1(uint16_t newTriangleY1);
	void setPointX2(uint16_t newTriangleX2);
	void setPointY2(uint16_t newTriangleY2);

private:
	struct parameters
	{
		union //param1
		{
			uint16_t width;
			uint16_t pnt_x1;
		};
		union //param2
		{
			uint16_t height;
			uint16_t pnt_y1;
		};
		union //param3
		{
			uint16_t radius;
			uint16_t pnt_x2;
		};
		union //param4
		{
			uint16_t pnt_y2;
		};	
	};

	primitives m_primitive;
	parameters m_parameters;
};

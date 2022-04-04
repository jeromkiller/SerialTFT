#include "paintPrimitivePacket.hpp"
#include "displaySerialBuffer.hpp"

paintPrimitivePacket::paintPrimitivePacket() :
	paintPacketBase(serialPacketBaseTypes::packetType::SHAPE_PACKET)
{
	m_primitive = primitives::point;
	memset(&m_parameters, 0, sizeof(parameters));
}

bool paintPrimitivePacket::serialize(displaySerialBuffer& buffer)
{
	if(!paintPacketBase::serialize(buffer))
	{
		return false;
	}

	buffer.add(m_primitive);
	buffer.add(m_parameters);
	return true;
}

bool paintPrimitivePacket::deserialize(displaySerialBuffer& buffer)
{
	bool ret = paintPacketBase::deserialize(buffer);
	
	if(!ret)
	{
		return false;
	}

	//read the data from where paint packet base left off
	buffer.get(&m_primitive);
	buffer.get(&m_parameters);
	return true;
}

//primitive is stored in the low nibble of the flags
paintPrimitivePacket::primitives paintPrimitivePacket::getPrimitiveFlag() const
{
	return static_cast<primitives>(m_flags.LowNibble);
}
void paintPrimitivePacket::setPrimitiveFlag(primitives primitive)
{
	m_flags.LowNibble = static_cast<uint8_t>(primitive);
}
//data getters and setter

uint16_t paintPrimitivePacket::getWidth() const
{
	return m_parameters.width;
}
uint16_t paintPrimitivePacket::getHeight() const
{
	return m_parameters.height;
}
uint16_t paintPrimitivePacket::getRadius() const
{
	return m_parameters.radius;
}
uint16_t paintPrimitivePacket::getPointX1() const
{
	return m_parameters.pnt_x1;
}
uint16_t paintPrimitivePacket::getPointY1() const
{
	return m_parameters.pnt_y1;
}
uint16_t paintPrimitivePacket::getPointX2() const
{
	return m_parameters.pnt_x2;
}
uint16_t paintPrimitivePacket::getPointY2() const
{
	return m_parameters.pnt_y2;
}
void paintPrimitivePacket::setWidth(uint16_t newWidth)
{
	m_parameters.width = newWidth;
	
}
void paintPrimitivePacket::setHeight(uint16_t newHeight)
{
	m_parameters.height = newHeight;
}
void paintPrimitivePacket::setRadius(uint16_t newRadius)
{
	m_parameters.radius = newRadius;
}
void paintPrimitivePacket::setPointX1(uint16_t newTriangleX1)
{
	m_parameters.pnt_x1 = newTriangleX1;
}
void paintPrimitivePacket::setPointY1(uint16_t newTriangleY1)
{
	m_parameters.pnt_y1 = newTriangleY1;
}
void paintPrimitivePacket::setPointX2(uint16_t newTriangleX2)
{
	m_parameters.pnt_x2 = newTriangleX2;
}
void paintPrimitivePacket::setPointY2(uint16_t newTriangleY2)
{
	m_parameters.pnt_y2 = newTriangleY2;
}
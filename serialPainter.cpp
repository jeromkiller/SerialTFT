#include "serialPainter.hpp"
#include "serialPacketBaseTypes.hpp"
#include "controlSerialPacket.hpp"
#include "paintPrimitivePacket.hpp"

#include "Adafruit_GFX.h"

serialPainter::serialPainter(Adafruit_GFX** displays, const uint8_t num_displays)
	: m_displays(displays)
{
	m_numDisplays = num_displays;
	m_penColor = 0;
	m_bgColor = 0;
	m_penX = 0;
	m_penY = 0;
}

serialPainter::~serialPainter()
{
	//free the screens
	//for(uint8_t d = 0; d < num_displays; d++)
	//{
	//	delete(m_displays + d);
	//}
	//m_displays = nullptr;
}

bool serialPainter::performCommand(const serialPacketBase& packet)
{
	using namespace serialPacketBaseTypes;

	switch(packet.getPacketType())
	{
		case packetType::CONTROL_PACKET :
		{
			const controlSerialPacket& controlPacket = static_cast<const controlSerialPacket&>(packet);
			
			if(controlPacket.getScreenSelectFlag())
			{
				m_activeScreens = controlPacket.getScreenSelect();
			}
			if(controlPacket.getScreenBrightnessFlag())
			{
				//TODO
			}
			if(controlPacket.getKeyBrightnessFlag())
			{
				//TODO
			}

			break;
		}
		case packetType::SHAPE_PACKET :
		{
			const paintPrimitivePacket& paintPacket = static_cast<const paintPrimitivePacket&>(packet);
			uint16_t future_penX = 0;
			uint16_t future_penY = 0;

			if(!setPaintBaseParameters(paintPacket))
			{
				return false;
			}

			//perform the draw command for every active display
			for(uint8_t d = 0; d < m_numDisplays; d++)
			{
				//check if the screen is active
				if(!((1 << d) & m_activeScreens))
				{
					continue;
				}

				Adafruit_GFX* current_disp = *(m_displays + d);

				switch(paintPacket.getPrimitiveFlag())
				{
					case paintPrimitivePacket::primitives::point :
					{
						current_disp->drawPixel(m_penX, m_penY, m_penColor.getColors());
						break;
					}
					case paintPrimitivePacket::primitives::line :
					{
						const uint16_t ptX1 = paintPacket.getPointX1();
						const uint16_t ptY1 = paintPacket.getPointY1();
						current_disp->drawLine(m_penX, m_penY, ptX1, ptY1, m_penColor.getColors());
						future_penX = ptX1;
						future_penY = ptY1;
						break;
					}
					case paintPrimitivePacket::primitives::circle :
					{
						const uint16_t rad = paintPacket.getRadius();
						current_disp->drawCircle(m_penX, m_penY, rad, m_penColor.getColors());
						break;
					}
					case paintPrimitivePacket::primitives::filledCircle :
					{
						const uint16_t rad = paintPacket.getRadius();
						current_disp->fillCircle(m_penX, m_penY, rad, m_penColor.getColors());
						break;
					}
					case paintPrimitivePacket::primitives::rectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						current_disp->drawRect(m_penX, m_penY, width, height, m_penColor.getColors());
						future_penX = m_penX + width;
						future_penY = m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::filledRectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						current_disp->fillRect(m_penX, m_penY, width, height, m_penColor.getColors());
						future_penX = m_penX + width;
						future_penY = m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::roundedRectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						const uint16_t radius = paintPacket.getRadius();
						current_disp->drawRoundRect(m_penX, m_penY, width, height, radius, m_penColor.getColors());
						future_penX += m_penX + width;
						future_penY += m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::filledRoundedRectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						const uint16_t radius = paintPacket.getRadius();
						current_disp->fillRoundRect(m_penX, m_penY, width, height, radius, m_penColor.getColors());
						future_penX += m_penX + width;
						future_penY += m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::triangle : 
					{
						const uint16_t ptX1 = paintPacket.getPointX1();
						const uint16_t ptY1 = paintPacket.getPointY1();
						const uint16_t ptX2 = paintPacket.getPointX2();
						const uint16_t ptY2 = paintPacket.getPointY2();
						current_disp->drawTriangle(m_penX, m_penY, ptX1, ptY1, ptX2, ptY2, m_penColor.getColors());
						future_penX = m_penX + ptX2;
						future_penY = m_penY + ptY2;
						break;
					}
					case paintPrimitivePacket::primitives::filledTriangle : 
					{
						const uint16_t ptX1 = paintPacket.getPointX1();
						const uint16_t ptY1 = paintPacket.getPointY1();
						const uint16_t ptX2 = paintPacket.getPointX2();
						const uint16_t ptY2 = paintPacket.getPointY2();
						current_disp->fillTriangle(m_penX, m_penY, ptX1, ptY1, ptX2, ptY2, m_penColor.getColors());
						future_penX = m_penX + ptX2;
						future_penY = m_penY + ptY2;
						break;
					}
					default:
					{
						return false;
					}
				}

			}
			m_penX = future_penX;
			m_penY = future_penY;
			break;
		}
		default:
		{
			return false;
		}
	}
	return true;
}

bool serialPainter::setPaintBaseParameters(const paintPacketBase& packet)
{
	if(packet.getCursorXFlag())
	{
		m_penX = packet.getCursorX();
	}
	if(packet.getCursorYFlag())
	{
		m_penY = packet.getCursorY();
	}
	if(packet.getPenColorFlag())
	{
		m_penColor = packet.getPenColor();
	}
	if(packet.getBgColorFlag())
	{
		m_penColor = packet.getBgColor();
	}

	return true;
}
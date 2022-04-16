#include "serialPainter.hpp"
#include "serialPacketBaseTypes.hpp"
#include "controlSerialPacket.hpp"
#include "paintPrimitivePacket.hpp"
#include "paintTextPacket.hpp"

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

Adafruit_GFX& serialPainter::getDisplay(uint8_t id)
{
	return **(m_displays + id);
}

bool serialPainter::performCommand(const serialPacketBase& packet)
{
	using namespace serialPacketBaseTypes;

	switch(packet.getPacketType())
	{
		case packetType::CONTROL_PACKET :
		{
			const controlSerialPacket& controlPacket = static_cast<const controlSerialPacket&>(packet);
			
			if(controlPacket.screenSelect.isSet())
			{
				m_activeScreens = controlPacket.screenSelect.value();
			}
			if(controlPacket.screenBrightness.isSet())
			{
				//TODO
			}
			if(controlPacket.keyBrightness.isSet())
			{
				//TODO
			}
			if(controlPacket.m_screenOrient.isSet())
			{
				const uint8_t orientation = controlPacket.m_screenOrient.value();
				for(uint8_t d = 0; d < m_numDisplays; d++)
				{
					//check if the screen is active
					if(!((1 << d) & m_activeScreens))
					{
						continue;
					}
					Adafruit_GFX& current_disp = getDisplay(d);
					current_disp.setRotation(orientation);
				}
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

				Adafruit_GFX& current_disp = getDisplay(d);

				switch(paintPacket.getPrimitiveFlag())
				{
					case paintPrimitivePacket::primitives::point :
					{
						current_disp.drawPixel(m_penX, m_penY, m_penColor.getColors());
						break;
					}
					case paintPrimitivePacket::primitives::line :
					{
						const uint16_t ptX1 = paintPacket.getPointX1();
						const uint16_t ptY1 = paintPacket.getPointY1();
						current_disp.drawLine(m_penX, m_penY, ptX1, ptY1, m_penColor.getColors());
						future_penX = ptX1;
						future_penY = ptY1;
						break;
					}
					case paintPrimitivePacket::primitives::circle :
					{
						const uint16_t rad = paintPacket.getRadius();
						current_disp.drawCircle(m_penX, m_penY, rad, m_penColor.getColors());
						break;
					}
					case paintPrimitivePacket::primitives::filledCircle :
					{
						const uint16_t rad = paintPacket.getRadius();
						current_disp.fillCircle(m_penX, m_penY, rad, m_bgColor.getColors());
						current_disp.drawCircle(m_penX, m_penY, rad, m_penColor.getColors());
						break;
					}
					case paintPrimitivePacket::primitives::rectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						current_disp.drawRect(m_penX, m_penY, width, height, m_penColor.getColors());
						future_penX = m_penX + width;
						future_penY = m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::filledRectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						current_disp.fillRect(m_penX, m_penY, width, height, m_bgColor.getColors());
						current_disp.drawRect(m_penX, m_penY, width, height, m_penColor.getColors());
						future_penX = m_penX + width;
						future_penY = m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::roundedRectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						const uint16_t radius = paintPacket.getRadius();
						current_disp.drawRoundRect(m_penX, m_penY, width, height, radius, m_penColor.getColors());
						future_penX = m_penX + width;
						future_penY = m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::filledRoundedRectangle :
					{
						const uint16_t width = paintPacket.getWidth();
						const uint16_t height = paintPacket.getHeight();
						const uint16_t radius = paintPacket.getRadius();
						current_disp.fillRoundRect(m_penX, m_penY, width, height, radius, m_bgColor.getColors());
						current_disp.drawRoundRect(m_penX, m_penY, width, height, radius, m_penColor.getColors());
						future_penX = m_penX + width;
						future_penY = m_penY + height;
						break;
					}
					case paintPrimitivePacket::primitives::triangle : 
					{
						const uint16_t ptX1 = paintPacket.getPointX1();
						const uint16_t ptY1 = paintPacket.getPointY1();
						const uint16_t ptX2 = paintPacket.getPointX2();
						const uint16_t ptY2 = paintPacket.getPointY2();
						current_disp.drawTriangle(m_penX, m_penY, ptX1, ptY1, ptX2, ptY2, m_penColor.getColors());
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
						current_disp.fillTriangle(m_penX, m_penY, ptX1, ptY1, ptX2, ptY2, m_bgColor.getColors());
						current_disp.drawTriangle(m_penX, m_penY, ptX1, ptY1, ptX2, ptY2, m_penColor.getColors());
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
		case packetType::TEXT_PACKET:
		{
			const paintTextPacket& textPacket = static_cast<const paintTextPacket&>(packet);
			uint16_t future_penX = 0;
			uint16_t future_penY = 0;

			//firstly set any of the base parameters
			if(!setPaintBaseParameters(textPacket))
			{
				return false;
			}

			//apply the base paremeters if they're set
			setTextBaseParameters(textPacket);

			//if we have to write text
			if(textPacket.textPtr.isSet())
			{
				//then write the text to screen for every active display
				for(uint8_t d = 0; d < m_numDisplays; d++)
				{
					//check if the screen is active
					if(!((1 << d) & m_activeScreens))
					{
						continue;
					}

					Adafruit_GFX& current_disp = getDisplay(d);
					current_disp.print(textPacket.textPtr.value());
				}
			}

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
	if(packet.cursorX.isSet())
	{
		m_penX = packet.cursorX.value();
	}
	if(packet.cursorY.isSet())
	{
		m_penY = packet.cursorY.value();
	}
	if(packet.penColor.isSet())
	{
		m_penColor = packet.penColor.value();
	}
	if(packet.bgColor.isSet())
	{
		m_bgColor = packet.bgColor.value();
	}

	return true;
}

//this sets the base parameters for writing text
bool serialPainter::setTextBaseParameters(const paintTextPacket& packet)
{
	for(uint8_t d = 0; d < m_numDisplays; d++)
	{
		//check if the screen is active
		if(!((1 << d) & m_activeScreens))
		{
			continue;
		}

		Adafruit_GFX& display = getDisplay(d);
		display.setCursor(m_penX, m_penY);
		display.setTextColor(m_penColor.getColors(), m_bgColor.getColors());

		if(packet.textSize.isSet())
		{
			const uint8_t textSize = packet.textSize.value();
			display.setTextSize(textSize);
		}

		if(packet.useWrapping.isSet())
		{
			const bool useTextWrap = packet.useWrapping.isSet();
			display.setTextWrap(useTextWrap);
		}
	}

	return true;
}
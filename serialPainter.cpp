#include "serialPainter.hpp"
#include "serialPacketBaseTypes.hpp"
#include "controlSerialPacket.hpp"
#include "paintPrimitivePacket.hpp"
#include "paintTextPacket.hpp"
#include "controlSerialPacket.hpp"
#include "paintSavedBMPPacket.hpp"

#include "Adafruit_SPITFT.h"
#include <SdFat.h>

serialPainter::serialPainter(Adafruit_SPITFT** displays, const uint8_t num_displays, SdFat* sd)
	: m_displays(displays)
	, m_sd(sd)
	, m_reader(Adafruit_ImageReader(*sd))
{
	m_numDisplays = num_displays;
	m_penColor = 0;
	m_textBgColor = 0;
	m_penX = 0;
	m_penY = 0;
	m_screenStyle = paintingTypes::MultiscreenStyles::STATIC;
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

Adafruit_SPITFT& serialPainter::getDisplay(uint8_t id)
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
					Adafruit_SPITFT& current_disp = getDisplay(d);
					current_disp.setRotation(orientation);
				}
			}
			if(controlPacket.m_multiStyle.isSet())
			{
				m_screenStyle = controlPacket.m_multiStyle.value();
			}

			break;
		}
		case packetType::SHAPE_PACKET :
		{
			const paintPrimitivePacket& paintPacket = static_cast<const paintPrimitivePacket&>(packet);
			uint16_t future_penX = 0;
			uint16_t future_penY = 0;

			setPaintPrimitiveParameters(paintPacket);

			//perform the draw command for every active display
			for(uint8_t d = 0; d < m_numDisplays; d++)
			{
				//check if the screen is active
				if(!((1 << d) & m_activeScreens))
				{
					continue;
				}

				Adafruit_SPITFT& current_disp = getDisplay(d);

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
						current_disp.fillCircle(m_penX, m_penY, rad, m_penColor.getColors());
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
						current_disp.fillRect(m_penX, m_penY, width, height, m_penColor.getColors());
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
						current_disp.fillRoundRect(m_penX, m_penY, width, height, radius, m_penColor.getColors());
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
						current_disp.fillTriangle(m_penX, m_penY, ptX1, ptY1, ptX2, ptY2, m_penColor.getColors());
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
			postDrawCommand();
			break;
		}
		case packetType::TEXT_PACKET:
		{
			const paintTextPacket& textPacket = static_cast<const paintTextPacket&>(packet);

			//apply the base paremeters if they're set
			setTextParameters(textPacket);

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

					Adafruit_SPITFT& current_disp = getDisplay(d);
					current_disp.print(textPacket.textPtr.value());
				}
			}
			postDrawCommand();
			break;
		}
		case packetType::BITMAP_PACKET:
		{
			const paintSavedBMPPacket& bmpPacket = static_cast<const paintSavedBMPPacket&>(packet);

			setBMPParameters(bmpPacket);

			if(bmpPacket.BMPPath.isSet())
			{
				const char* bmpPath = bmpPacket.BMPPath.value();
				//cache the bitmap,
				ImageReturnCode stat;
				Adafruit_Image sprite; 
				stat = m_reader.loadBMP(bmpPath, sprite);		
	
				if(stat != IMAGE_SUCCESS)
				{
					return false;
				}

				//set the transparant color to the bg color
				GFXcanvas16* sprite_canvas = static_cast<GFXcanvas16*>(sprite.getCanvas());
				uint16_t* sprite_buff = sprite_canvas->getBuffer();
				const uint16_t s_height = sprite_canvas->height();
				const uint16_t s_width = sprite_canvas->width();
				for(uint16_t i = 0; i < s_height * s_width; i++)
				{
					if(sprite_buff[i] == m_bmpTransColor.getColors())
					{
						sprite_buff[i] = m_bmpBgColor.getColors();
					}
				}

				//then draw the bitmap
				for(uint8_t d = 0; d < m_numDisplays; d++)
				{
					//check if the screen is active
					if(!((1 << d) & m_activeScreens))
					{
						continue;
					}

					Adafruit_SPITFT& current_disp = getDisplay(d);
					sprite.draw(current_disp, m_penX, m_penY);
				}

				//save where the cursor is going to end up at
				m_penX += s_width;
				m_penY += s_height;
			}

			postDrawCommand();
			break;
		}
		default:
		{
			return false;
		}
	}
	return true;
}

void serialPainter::setPaintBaseParameters(const paintPacketBase& packet)
{
	if(packet.cursorX.isSet())
	{
		m_penX = packet.cursorX.value();
	}
	if(packet.cursorY.isSet())
	{
		m_penY = packet.cursorY.value();
	}

}

void serialPainter::setPaintPrimitiveParameters(const paintPrimitivePacket& packet)
{
	setPaintBaseParameters(packet);

	if(packet.penColor.isSet())
	{
		m_penColor = packet.penColor.value();
	}
}

//this sets the base parameters for writing text
void serialPainter::setTextParameters(const paintTextPacket& packet)
{
	setPaintBaseParameters(packet);

	for(uint8_t d = 0; d < m_numDisplays; d++)
	{
		//check if the screen is active
		if(!((1 << d) & m_activeScreens))
		{
			continue;
		}

		Adafruit_GFX& display = getDisplay(d);
		display.setCursor(m_penX, m_penY);

		if(packet.penColor.isSet())
		{
			const packedColor tColor = packet.penColor.value();
			m_textColor = tColor;
		}

		if(packet.bgColor.isSet())
		{
			const packedColor bgColor = packet.bgColor.value();
			m_textBgColor = bgColor;
		}

		display.setTextColor(m_penColor.getColors(), m_textBgColor.getColors());

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
}

void serialPainter::setBMPParameters(const paintSavedBMPPacket& packet)
{
	setPaintBaseParameters(packet);

	if(packet.bgColor.isSet())
	{
		const packedColor color = packet.bgColor.value();
		m_bmpBgColor = color;
	}

	if(packet.transColor.isSet())
	{
		const packedColor color = packet.transColor.value();
		m_bmpTransColor = color;
	}
}

//TODO: testme
void serialPainter::postDrawCommand()
{
	if(m_screenStyle == paintingTypes::MultiscreenStyles::SHIFTING)
	{
		//shift the display numbers around
		bool outbit = m_activeScreens & (1 << (m_numDisplays -1));
		m_activeScreens <<= 1;
		if(outbit)
		{
			outbit |= 1;
		}
	}
}
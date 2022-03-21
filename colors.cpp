#include "colors.hpp"

packedColor::packedColor()
{
	m_packedColors.rawColor = 0;
}

packedColor::packedColor(uint16_t col)
{
	m_packedColors.rawColor = col;
}

uint8_t packedColor::getRed() const
{
	return m_packedColors.red << 3;
}

uint8_t packedColor::getGreen() const
{
	return m_packedColors.green << 2;
}

uint8_t packedColor::getBlue() const
{
	return m_packedColors.blue << 3;
}

void packedColor::setRed(const uint8_t newRed)
{
	m_packedColors.red = newRed >> 3;
}

void packedColor::setGreen(const uint8_t newGreen)
{
	m_packedColors.green = newGreen >> 2;
}

void packedColor::setBlue(const uint8_t newBlue)
{
	m_packedColors.blue = newBlue >> 3;
}

packedColor& packedColor::operator=(const uint16_t newColors)
{
	setColors(newColors);
	return *this;
}

uint16_t  packedColor::getColors() const
{
	return m_packedColors.rawColor;
}

void packedColor::setColors(const uint16_t newColors)
{
	m_packedColors.rawColor = newColors;
}
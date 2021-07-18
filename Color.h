#pragma once

#include <cmath>

class Color
{
	double m_red, m_green, m_blue, m_special;

public:
	Color();

	Color(double, double, double, double);

	// method functions

	double getColorRed() const { return m_red; }
	double getColorGreen() const { return m_green; }
	double getColorBlue() const { return m_blue; }
	double getColorSpecial() const { return m_special; }

	double setColorRed(double redValue) { return m_red = redValue; }
	double setColorGreen(double greenValue) { return m_green = greenValue; }
	double setColorBlue(double BlueValue) { return m_blue = BlueValue; }
	double setColorSpecial(double SpecialValue) { return m_special = SpecialValue; }

	double brightness() const
	{
		return (m_red + m_green + m_blue) / 3;
	}

	Color colorScalar(double scalar) const
	{
		return {m_red * scalar, m_green * scalar, m_blue * scalar, m_special};
	}

	Color colorAdd(Color color) const
	{
		return {m_red + color.getColorRed(), m_green + color.getColorGreen(), m_blue + color.getColorBlue(), m_special};
	}

	Color colorMultiply(Color color) const
	{
		return {m_red * color.getColorRed(), m_green * color.getColorGreen(), m_blue * color.getColorBlue(), m_special};
	}

	Color colorAverage(Color color) const
	{
		return {(m_red + color.getColorRed()) / 2, (m_green + color.getColorGreen()) / 2, (m_blue + color.getColorBlue()) / 2, m_special};
	}

	Color clip()
	{
		double all_light = m_red + m_green + m_blue;
		double excess_light = all_light - 3;

		if (excess_light > 0)
		{
			m_red = m_red + excess_light * (m_red / all_light);
			m_green = m_green + excess_light * (m_green / all_light);
			m_blue = m_blue + excess_light * (m_blue / all_light);
		}

		if (m_red > 1) { m_red = 1; }
		if (m_green > 1) { m_green = 1; }
		if (m_blue > 1) { m_blue = 1; }
		if (m_red < 0) { m_red = 0; }
		if (m_green < 0) { m_green = 0; }
		if (m_blue < 0) { m_blue = 0; }

		return {m_red, m_green, m_blue, m_special};
	}
};

Color::Color()
	: m_red(0.5), m_green(0.5), m_blue(0.5), m_special(0)
{
}

Color::Color(double red, double green, double blue, double special)
	: m_red(red), m_green(green), m_blue(blue), m_special(special)
{
}

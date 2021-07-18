#pragma once

#include "Vect.h"
#include "Color.h"
#include "Source.h"

class Light : public Source
{
	Vect m_postion;
	Color m_color;

public:

	Light();

	Light(Vect, Color);

	// method functions

	Vect getLightPosition() override { return m_postion; }
	Color getLightColor() override { return m_color; }
};

Light::Light()
	: m_postion(Vect(0, 0, 0)), m_color(Color(1, 1, 1, 0))
{
}

Light::Light(Vect postion, Color color)
	: m_postion(postion), m_color(color)
{
}

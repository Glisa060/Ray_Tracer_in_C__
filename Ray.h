#pragma once

#include "Vect.h"

class Ray
{
	Vect m_origin, m_direction;

public:

	Ray();

	Ray(Vect, Vect);

	// method functions

	Vect getRayOrigin() { return m_origin; }
	Vect getRayDirection() { return m_direction; }
};

Ray::Ray()
	: m_origin(Vect(0, 0, 0)), m_direction(Vect(1, 0, 0))
{
}

Ray::Ray(Vect origin, Vect direction)
	: m_origin(origin), m_direction(direction)
{
}
#pragma once

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object
{
public:

	Object();

	// method functions
	virtual Color getColor() { return {0.0, 0.0, 0.0, 0}; }

	virtual double findIntersection(Ray ray)
	{
		return 0;
	}

	virtual Vect getNormalAt(Vect intersection_position)
	{
		return {0, 0, 0};
	}
};

Object::Object()

= default;
#pragma once

#include "Vect.h"
#include "Color.h"

class Source
{
public:

	Source();

	// method functions
	virtual Vect getLightPosition() { return {0, 0, 0}; }
	virtual Color getLightColor() { return {1, 1, 1, 0}; }
};

Source::Source()
= default;
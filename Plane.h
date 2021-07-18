#pragma once

#include <cmath>
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object
{
	Vect m_normal;
	double m_distance;
	Color m_color;

public:

	Plane();

	Plane(Vect, double, Color);

	// method functions

	Vect getPlaneNormal() { return m_normal; }
	double getPlaneDistance() const { return m_distance; }
	Color getColor() override { return m_color; }

	Vect getNormalAt(Vect point) override
	{
		return m_normal;
	}

	double findIntersection(Ray ray) override
	{
		Vect ray_direction = ray.getRayDirection();
		double ray_point = ray_direction.dotProduct(m_normal);
		if (ray_point == 0)
		{
			// Ray is parallel to the plane.
			return -1;
		}
		else
		{
			double intersection = m_normal.dotProduct(ray.getRayOrigin().vectAdd(m_normal.vectMult(m_distance).negative()));
			return -1 * intersection / ray_point;
		}
	}
};

Plane::Plane()
	: m_normal(Vect(1, 0, 0)), m_distance(0), m_color(Color(0.5, 0.5, 0.5, 0))
{
}

Plane::Plane(Vect normal, double distance, Color color)
	: m_normal(normal), m_distance(distance), m_color(color)
{
}

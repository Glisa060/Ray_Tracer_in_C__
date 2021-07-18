#pragma once

#include <cmath>
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Sphere : public Object
{
	Vect m_center;
	double m_radius;
	Color m_color;

public:

	Sphere();

	Sphere(Vect, double, Color);

	// method functions

	Vect getSphereCenter() { return m_center; }
	double getSphereRadius() const { return m_radius; }
	Color getColor() override { return m_color; }

	Vect getNormalAt(Vect point) override
	{
		// Normal always points away from the center of a sphere.
		Vect normal_Vect = point.vectAdd(m_center.negative()).normalize();
		return normal_Vect;
	}

	double findIntersection(Ray ray) override
	{
		Vect ray_origin = ray.getRayOrigin();
		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		Vect ray_direction = ray.getRayDirection();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();

		Vect sphere_center = m_center;
		double sphere_center_x = sphere_center.getVectX();
		double sphere_center_y = sphere_center.getVectY();
		double sphere_center_z = sphere_center.getVectZ();

		double a = 1; // Normalized
		double b = (2 * (ray_origin_x - sphere_center_x) * ray_direction_x) + (2 * (ray_origin_y - sphere_center_y) * ray_direction_y) + (2 * (ray_origin_z - sphere_center_z) * ray_direction_z);
		double c = pow(ray_origin_x - sphere_center_x, 2) + pow(ray_origin_y - sphere_center_y, 2) + pow(ray_origin_z - sphere_center_z, 2) - (m_radius * m_radius);
		double discriminant = b * b - 4 * c;

		if (discriminant > 0)
		{
			// The ray intersects the sphere.

			// The first root
			double root_1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.000001;

			if (root_1 > 0)
			{
				// The first root is the smallest positive root.
				return root_1;
			}
			else
			{
				// The second root is the smallest positive root.
				double root_2 = ((sqrt(discriminant) - b) / 2) - 0.000001;
				return root_2;
			}
		}
		else
		{
			// The ray missed the Sphere.
			return -1;
		}
	}
};

Sphere::Sphere()
	: m_center(Vect(0, 0, 0)), m_radius(1.0), m_color(Color(0.5, 0.5, 0.5, 0))
{
}

Sphere::Sphere(Vect center, double radius, Color color)
	: m_center(center), m_radius(radius), m_color(color)
{
}

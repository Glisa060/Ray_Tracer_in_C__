#pragma once

#include <cmath>

class Vect
{
	double m_x, m_y, m_z;

public:
	Vect();

	Vect(double, double, double);

	// method functions

	double getVectX() const { return m_x; }
	double getVectY() const { return m_y; }
	double getVectZ() const { return m_z; }

	double magnitude() const
	{
		return sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
	}

	Vect normalize() const
	{
		double magnitude = sqrt((m_x * m_x) + (m_y * m_y) + (m_z * m_z));
		return {m_x / magnitude, m_y / magnitude, m_z / magnitude};
	}

	Vect negative() const
	{
		return {-m_x, -m_y, -m_z};
	}

	double dotProduct(Vect vector) const
	{
		return m_x * vector.getVectX() + m_y * vector.getVectY() + m_z * vector.getVectZ();
	}

	Vect crossProduct(Vect vector) const
	{
		return {m_y * vector.getVectZ() - m_z * vector.getVectY(), m_z * vector.getVectX() - m_x * vector.getVectZ(), m_x * vector.getVectY() - m_y * vector.getVectX()};
	}

	Vect vectAdd(Vect vector) const
	{
		return {m_x + vector.getVectX(), m_y + vector.getVectY(), m_z + vector.getVectZ()};
	}

	Vect vectMult(double scalar) const
	{
		return {m_x * scalar, m_y * scalar, m_z * scalar};
	}
};

Vect::Vect()
	: m_x(0), m_y(0), m_z(0)
{
}

Vect::Vect(double i, double j, double k)
	: m_x(i), m_y(j), m_z(k)
{
}

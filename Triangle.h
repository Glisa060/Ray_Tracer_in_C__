#pragma once

#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Triangle : public Object
{
	Vect A, B, C;
	Vect m_normal;
	double m_distance{};
	Color m_color;

public:

	Triangle();

	Triangle(Vect, Vect, Vect, Color);

	// method functions

	Vect getTriangleNormal() 
	{ 
		Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
		Vect BA(B.getVectX() - A.getVectX(), B.getVectY() - A.getVectY(), B.getVectZ() - A.getVectZ());
		m_normal = CA.crossProduct(BA).normalize();
		return m_normal;
	}
	
	double getTriangleDistance() 
	{ 
		m_normal = getTriangleNormal();
		m_distance = m_normal.dotProduct(A);
		return m_distance; 
	}
	
	Color getColor() override { return m_color; }

	Vect getNormalAt(Vect point) override
	{
		m_normal = getTriangleNormal();
		return m_normal;
	}

	double findIntersection(Ray ray) override
	{
		Vect ray_direction = ray.getRayDirection();
		Vect ray_origin = ray.getRayOrigin();
		m_normal = getTriangleNormal();
		m_distance = getTriangleDistance();
		double ray_point = ray_direction.dotProduct(m_normal);
		
		if (ray_point == 0)
		{
			// Ray is parallel to the Triangle.
			return -1;
		}
		else
		{
			double intersection = m_normal.dotProduct(ray.getRayOrigin().vectAdd(m_normal.vectMult(m_distance).negative()));
			double plane_distance = -1 * intersection / ray_point;

			double Qx = ray_direction.vectMult(plane_distance).getVectX() + ray_origin.getVectX();
			double Qy = ray_direction.vectMult(plane_distance).getVectY() + ray_origin.getVectY();
			double Qz = ray_direction.vectMult(plane_distance).getVectZ() + ray_origin.getVectZ();

			Vect Q(Qx, Qy, Qz);

			Vect CA(C.getVectX() - A.getVectX(), C.getVectY() - A.getVectY(), C.getVectZ() - A.getVectZ());
			Vect QA(Q.getVectX() - A.getVectX(), Q.getVectY() - A.getVectY(), Q.getVectZ() - A.getVectZ());
			double test_1 = (CA.crossProduct(QA)).dotProduct(m_normal);


			Vect BC(B.getVectX() - C.getVectX(), B.getVectY() - C.getVectY(), B.getVectZ() - C.getVectZ());
			Vect QC(Q.getVectX() - C.getVectX(), Q.getVectY() - C.getVectY(), Q.getVectZ() - C.getVectZ());
			double test_2 = (BC.crossProduct(QC)).dotProduct(m_normal);
			

			Vect AB(A.getVectX() - B.getVectX(), A.getVectY() - B.getVectY(), A.getVectZ() - B.getVectZ());
			Vect QB(Q.getVectX() - B.getVectX(), Q.getVectY() - B.getVectY(), Q.getVectZ() - B.getVectZ());
			double test_3 = (AB.crossProduct(QB)).dotProduct(m_normal);

			if ((test_1 >= 0) && (test_2 >= 0) && (test_3 >= 0))
			{
				// Inside Triangle.
				return -1 * intersection / ray_point;
			}
			else
			{
				//Outside of Triangle.
				return -1;
			}
		}
	}
};

Triangle::Triangle()
	: A(Vect(1, 0, 0)), B(Vect(0, 1, 0)), C(Vect(0, 0, 1)), m_color(Color(0.5, 0.5, 0.5, 0))
{

}

Triangle::Triangle(Vect point_a,Vect point_b, Vect point_c, Color color)
	: A(point_a), B(point_b), C(point_c), m_color(color)
{

}

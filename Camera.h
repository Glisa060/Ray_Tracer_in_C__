#pragma once

#include "Vect.h"

class Camera
{
	Vect m_campos, m_camdir, m_camright, m_camdown;

public:

	Camera();

	Camera(Vect, Vect, Vect, Vect);

	// method functions

	Vect getCameraPosition() { return m_campos; }
	Vect getCameraDirection() { return m_camdir; }
	Vect getCameraRight() { return m_camright; }
	Vect getCameraDown() { return m_camdown; }
};

Camera::Camera()
	: m_campos(Vect(0, 0, 0)), m_camdir(Vect(0, 0, 1)), m_camright(Vect(0, 0, 0)), m_camdown(Vect(0, 0, 0))
{
}

Camera::Camera(Vect campos, Vect camdir, Vect camright, Vect camdown)
	: m_campos(campos), m_camdir(camdir), m_camright(camright), m_camdown(camdown)
{
}
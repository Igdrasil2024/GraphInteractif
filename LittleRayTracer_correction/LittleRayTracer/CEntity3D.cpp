#include "Utils.h"
#include "CEntity3D.h"


Entity3D::Entity3D() :
	m_position(0, 0, 0),
	m_orientation(1, 0, 0, 0),
	m_model(glm::mat4(1.0f)), 
	m_modelInverse(glm::mat4(1.0f))
{
}

Entity3D::~Entity3D()
{

}
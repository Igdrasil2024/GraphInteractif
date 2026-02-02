/// \file	Camera.cpp
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024
/// 

#include "stdafx.h"
#include "Camera.h"


CCamera::CCamera(std::string p_name) :C3DNode(p_name),
	m_nearFarPlanes(0.1f, 100.0f)
{
	// default projection (16/9 with vertical FOV = 90°)
	m_projection = glm::frustum(-m_nearFarPlanes.x* 16.0f / 9.0f, m_nearFarPlanes.x* 16.0f / 9.0f, -m_nearFarPlanes.x, m_nearFarPlanes.x, m_nearFarPlanes.x, m_nearFarPlanes.y);
}


CCamera::~CCamera()
{

}

void CCamera::setNearFarPlanes(float p_near, float p_far)
{
	m_nearFarPlanes.x = glm::min(p_near,p_far);
	m_nearFarPlanes.y = glm::max(p_near, p_far);
}

glm::vec2 CCamera::getNearFarPlanes()
{
	return m_nearFarPlanes;
}



void CCamera::setProjection(float p_verticalFoV, float p_ratio)
{
	m_projection = glm::perspective(p_verticalFoV,  p_ratio, m_nearFarPlanes.x, m_nearFarPlanes.y);
}

void CCamera::setProjection(glm::mat4 &p_projection)
{
	m_projection = p_projection;
}

glm::mat4 CCamera::getProjection()
{
	
return m_projection;
	
}


void CCamera::render(glm::mat4& p_model, glm::mat4 &p_view, glm::mat4 &p_projection)
{
	// render what you want : referential / frustum / focal plane ...
	// only useful if you want to render something to represent the camera
}

glm::mat4 CCamera::getView()
{

	return glm::inverse(getTransform());
	
}


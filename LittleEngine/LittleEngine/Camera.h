/// \file	CCamera.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#ifndef _CAMERA_
#define _CAMERA_

#include "stdafx.h"
#include "C3DNode.h"


/// \class CCamera
/// \brief to store the camera params
///
/// This class is a child of C3DNode to add it lights params 
///
class CCamera :
	public C3DNode
{
public:
	/// \brief constructor
	///
	/// \param[in]	p_name	the name of the camera
	///
	CCamera(std::string p_name = "--Camera--");
	virtual ~CCamera();

	/// \brief to set the near & far planes
	///
	/// \param[in]	p_near	the near plane
	/// \param[in]	p_far	the far plane
	///
	void setNearFarPlanes(float p_near, float p_far);
	/// \brief to get the near & far planes
	///
	/// \return	the near and far plane into a vec2 (x = near, y= far)
	///
	glm::vec2 getNearFarPlanes();

	/// \ brief to set the projection matrix using simple params (symetric frustum) : You MUST set the near/far planes bafore calling this method
	///
	/// \param[in]	p_verticalFoV 	the vertical field of view , PI/2 by default
	/// \param[in]	p_ratio			the ratio (W/H) 
	/// \param[in]	p_near			the near plane
	/// \param[in]	p_far			the far plane
	///
	void setProjection(float p_verticalFoV = M_PI/2.0f, float p_ratio = 16.0f/9.0f);
	/// \brief to set the projection giving the matrix.
	///
	/// \param[in]	p_projection	the projection matrix to set
	///
	void setProjection(glm::mat4 &p_projection);
	/// \brief to get the projection matrix. You MUST have set the near/far planes before calling this method if camera has got a focal plane
	///
	/// \return	the matrix used for projection
	///
	glm::mat4 getProjection();
	
	/// \brief draw the camera
	///
	/// \param[in]	p_model			the model matrix (local transform already used to generate this matrix in the C3DNode draw method)
	/// \param[in]	p_view			the view matrix
	/// \param[in]	p_projection	the projection matrix
	///
	virtual void render(glm::mat4 &p_model, glm::mat4 &p_view, glm::mat4 &p_projection);

	/// \brief to get the view matrix from this camera
	/// 
	/// \return	the view matrix to fill
	///
	glm::mat4 getView();


protected:
	glm::vec2		m_nearFarPlanes;	///< to set the near & far planes 

	glm::mat4		m_projection;		///< the projection matrix
};


#endif

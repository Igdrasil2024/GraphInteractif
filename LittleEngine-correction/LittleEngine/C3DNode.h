/// \file	C3DNode.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024
/// 

#ifndef _3DNODE_
#define _3DNODE_

#include "stdafx.h"


/// \class C3DNode
/// \brief to manage a 3D node
///
/// This class is to be derived into others to be specialized as 3D object, light, camera ...
///
class C3DNode
{
public:
	/// \brief constructor
	///
	/// \param[in]	p_name	the name of the node
	///
	C3DNode(std::string p_name = "--Node--");
	virtual ~C3DNode();

	/// \brief to get the name of the node
	///
	/// \return the name of the node
	///
	std::string getName();

	/// \brief to change the name of the node
	/// 
	/// \param[in]	p_name	the new name of the node
	///
	void setName(std::string &p_name);

	
	/// \brief to set the matrix regarding another node (in the specified node referential)
	///
	/// \param[in]	p_transform		the trasnform matrix 
	///	\param[in]	p_ref			the node to use as referential (nullptr by default => set the Model matrix in this case)
	///
	void setTransform(glm::mat4 p_transform, C3DNode * p_ref = nullptr);
	/// \brief to get the transform of an object in the given referential
	/// 
	/// \param[in	p_ref	the referential in which trasnform will be calculated (nullptr by default => should be model matrix)
	///
	//// \return	the transform matrix in the referential p_ref
	///
	glm::mat4 getTransform(C3DNode * p_ref = nullptr);

	/// \brief draw the node and its children (recursive)
	///
	/// \param[in]	p_model			the current transform matrix. Need to be multiplied by the local transform matrix (model = m_localTransform * p_model)
	/// \param[in]	p_view			the view matrix
	/// \param[in]	p_projection	the projection matrix
	///
	void draw(glm::mat4 &p_model, glm::mat4 &p_view, glm::mat4 &p_projection);


	/// \brief to render the node (virtual method so it can call the derived one on C3DObject, CLight, CCamera, CFocalPlane ...)
	///
	/// \param[in]	p_model			the model matrix
	/// \param[in]	p_view			the view matrix
	/// \param[in]	p_projection	the projection matrix
	///
	virtual void render(glm::mat4& p_model, glm::mat4 &p_view, glm::mat4 &p_projection);


	/// \brief to delete all the nodes still in memory
	static void deleteAllNodes();

protected:
	std::string				m_name;				///< the name of the node
	glm::mat4				m_transform;		///< the transformation matrix (= model matrix in our use case)

	static std::vector<C3DNode*>	m_nodeList;	///< list of all nodes created in memory
};


#endif

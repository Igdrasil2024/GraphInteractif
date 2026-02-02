/// \file	C3DObject.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024
/// 

#ifndef _3DOBJECT_
#define _3DOBJECT_

#include "stdafx.h"
#include "C3DNode.h"
#include "Mesh.h"

///	\class	C3DObject
///	\brief	to manage a 3D object 
///
/// this class enables us to store the mesh, the material, the parent object and the local matrix of the object
///

class C3DObject: 
	public C3DNode
{
public:
	/// \brief constructor
	///
	/// \param[in]	p_name	the name of the object
	///
	C3DObject(std::string p_name = "--Object--");
	/// \brief destructor
	virtual ~C3DObject();

	/// \brief draw the object
	///
	/// \param[in]	p_model			the model matrix (local transform already used to generate this matrix in the C3DNode draw method)
	/// \param[in]	p_view			the view matrix
	/// \param[in]	p_projection	the projection matrix
	///
	virtual void render(glm::mat4 &p_model, glm::mat4 &p_view, glm::mat4 &p_projection);

	/// \brief to get the number of mesh in the current object
	///
	/// \return the number of meshes
	///
	unsigned int getNbMeshes();
	/// \brief to add a mesh to the object 
	///
	///	\param[in] p_mesh	the mesh to set into the object. Be careful, the object does not delete the mesh since it can be instanciated in many objects
	///
	void addMesh(CMesh * p_mesh);
	/// \brief to get the mesh 
	///
	/// \param[in] p_index	the index of the mesh in the mesh list
	///
	///	\return the mesh address
	///
	CMesh * getMesh(unsigned int p_index);

protected:
	std::vector<CMesh *>	m_meshes;			///< meshes of the object

};

#endif
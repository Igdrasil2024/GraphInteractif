#include "stdafx.h"
#include "C3DObject.h"




C3DObject::C3DObject(std::string p_name):C3DNode(p_name)
{
}


C3DObject::~C3DObject()
{

}

void C3DObject::render(glm::mat4 &p_model, glm::mat4 &p_view, glm::mat4 &p_projection)
{
	// FOKS LAB ---
	
	glMatrixMode(GL_MODELVIEW); // pas utile => on y est toujours
	glm::mat4 mv = p_view * p_model;
	glLoadMatrixf(&mv[0][0]);

	// loop on all the meshes and draw each one
	std::vector<CMesh*>::iterator it = m_meshes.begin();
	for (;it != m_meshes.end();it++)
	{
		CMesh* m = (*it);
		m->draw(p_model, p_view, p_projection);
	}
}

unsigned int C3DObject::getNbMeshes()
{
	return m_meshes.size();
}


void C3DObject::addMesh(CMesh * p_mesh)
{
	if (p_mesh)
		m_meshes.push_back(p_mesh);
}

CMesh * C3DObject::getMesh(unsigned int p_index)
{
	if (p_index >= m_meshes.size())
		return nullptr;

	return m_meshes[p_index];
}

/// \file	C3DNode.cpp
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024
/// 
/// 

#include "stdafx.h"
#include "C3DNode.h"


std::vector<C3DNode*> C3DNode::m_nodeList;

C3DNode::C3DNode(std::string p_name):
	m_name(p_name),
	m_transform(glm::mat4(1.0f))
{
	// add to the node list in memory
	m_nodeList.push_back(this);
}


C3DNode::~C3DNode()
{
	// remove from the nodelist in memory
	std::vector<C3DNode*>::iterator found = std::find(m_nodeList.begin(), m_nodeList.end(), this);
	if (found != m_nodeList.end())
		m_nodeList.erase(found);
}


std::string C3DNode::getName()
{
	return m_name;
}

void C3DNode::setName(std::string &p_name)
{
	m_name = p_name;
}


void C3DNode::setTransform(glm::mat4 p_transform, C3DNode * p_ref)
{
	// FOKS LAB ---
	
	// to set a matrix (pose) regarding another object (reference matrix) you only need to multiply the wanted pose (matrix) to the reference matrix
	// BE CAREFUL : res = refMatrix * transform NOT res = transform * refMatrix
	if (!p_ref)
		m_transform = p_transform;
	else
		m_transform = p_ref->getTransform() * p_transform;
}

glm::mat4 C3DNode::getTransform(C3DNode * p_ref)
{
	// FOKS LAB ---
	
	// to get a pose (matrix) regarding another object (reference matrix) you need to inverse the reference matrix and multiply it by the object trasnform
	if (!p_ref)
		return m_transform;
	return glm::inverse(p_ref->getTransform()) * m_transform;
	
}



void C3DNode::draw(glm::mat4 &p_worldTransform, glm::mat4 &p_view, glm::mat4 &p_projection)
{
	// FOKS LAB ---

	// Calculate the new model matrix using the local transform
	// "recursively" : you know the parent matrix, you need to set the matrix of the object (transform is to be the "local" matrix)


	glm::mat4 model(1.0f);
	model = p_worldTransform * m_transform;

	// render the node with the new model matrix
	render(model, p_view, p_projection);
}

void C3DNode::render(glm::mat4& p_model, glm::mat4 &p_view, glm::mat4 &p_projection)
{
	// Nothing there to render, you can choose to render a referential to show the transformation of the node
	// virtual method, could have been a virtual pure method 
}


void C3DNode::deleteAllNodes()
{
	while (m_nodeList.size() != 0)
	{
		delete m_nodeList.back();
	}
}
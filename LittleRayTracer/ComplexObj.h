#pragma once
#include "CEntity3D.h"
#include "Collider.h"
#include <vector>
#include <string>

struct vertex {
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_texcoord;
};

struct face {
    unsigned int index[3];
};

class ComplexObj :
    public Entity3D,
    public Collider
	
{
public :
    ComplexObj(std::string p_OBJpath);

    virtual RayCastResult raycast(Ray& p_ray, glm::vec2 p_tInterval);
    
protected :
    RayCastResult raycastTriangle(Ray& p_ray, glm::vec2 p_tInterval, glm::vec3 A, glm::vec3 B, glm::vec3 C);
    bool loadFromOBJ(const std::string& path);

    std::vector<vertex> m_vertexArray;
    std::vector<face>   m_faceArray;
};






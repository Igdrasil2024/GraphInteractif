#pragma once
#include "CEntity3D.h"
#include "Collider.h"


class ComplexObj :
    public Entity3D,
    public Collider
{
public :
    ComplexObj(std::string p_OBJpath);

    virtual RayCastResult raycast(Ray& p_ray, glm::vec2 p_tInterval);
    
protected :
    RayCastResult raycastTriangle(Ray& p_ray, glm::vec2 p_tInterval, glm::vec3 A, glm::vec3 B, glm::vec3 C);

};


#include "ComplexObj.h"
#include "../LittleEngine/LittleEngine/Mesh.h"

//Constructeur ==> cf. CMesh
ComplexObj::ComplexObj(std::string p_OBJpath)
{
	CMesh* mesh = CMesh::loadFromOBJ(p_OBJpath);
	if (mesh)
	{
		// On copie les données dans ComplexObj
		m_vertexArray = mesh->m_vertexArray;
		m_faceArray = mesh->m_faceArray;
		// On peut supprimer le mesh si on ne veut pas le garder
		// delete mesh;
	}
}


//Pour charger l'objet :  LoadFromOBJ cf. CMesh 
// il faut "nettoyer" CMesh pour créer la classe ComplexObj



RayCastResult ComplexObj::raycastTriangle(Ray& p_ray, glm::vec2 p_tInterval, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    const float EPSILON = 1e-6f;
    glm::vec3 edge1 = B - A;
    glm::vec3 edge2 = C - A;
    glm::vec3 h = glm::cross(p_ray.direction, edge2);
    float a = glm::dot(edge1, h);

    if (std::abs(a) < EPSILON)
        return RayCastResult(); 

    float f = 1.0f / a;
    glm::vec3 s = p_ray.origin - A;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f)
        return RayCastResult();

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(p_ray.direction, q);
    if (v < 0.0f || u + v > 1.0f)
        return RayCastResult();

    float t = f * glm::dot(edge2, q);
    if (t < p_tInterval.x || t > p_tInterval.y)
        return RayCastResult();

    RayCastResult result;
    result.hit = true;
    result.t = t;
    result.position = p_ray.origin + t * p_ray.direction;
    result.normal = glm::normalize(glm::cross(edge1, edge2));
    result.u = u;
    result.v = v;
    return result;
}

RayCastResult ComplexObj::raycast(Ray& p_ray, glm::vec2 p_tInterval)
{

    RayCastResult closestHit;
    closestHit.t = std::numeric_limits<float>::max();
    closestHit.hit = false;

    for (const auto& f : m_faceArray)
    {
        const vertex& vA = m_vertexArray[f.index[0]];
        const vertex& vB = m_vertexArray[f.index[1]];
        const vertex& vC = m_vertexArray[f.index[2]];

        RayCastResult hit = raycastTriangle(p_ray, p_tInterval, vA.m_position, vB.m_position, vC.m_position);
        if (hit.hit && hit.t < closestHit.t)
        {
            closestHit = hit;
            closestHit.normal = glm::normalize(
                (1.0f - hit.u - hit.v) * vA.m_normal +
                hit.u * vB.m_normal +
                hit.v * vC.m_normal
            );
            closestHit.uv =
                (1.0f - hit.u - hit.v) * vA.m_texcoord[0] +
                hit.u * vB.m_texcoord[0] +
                hit.v * vC.m_texcoord[0];
        }
    }

    return closestHit;
}
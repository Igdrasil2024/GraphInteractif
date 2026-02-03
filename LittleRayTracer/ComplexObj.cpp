#include "ComplexObj.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include "./ThirdPartyLibs/glm/glm/glm.hpp"
#include "./ThirdPartyLibs/glm/glm/gtx/norm.hpp" 
#include "./ThirdPartyLibs/glm/glm/gtc/type_ptr.hpp"

//Constructeur ==> cf. CMesh
bool ComplexObj::loadFromOBJ(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {

       
        return false;
    }

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;

    m_vertexArray.clear();
    m_faceArray.clear();

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "v") {
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            positions.push_back(v);
        }
        else if (token == "vn") {
            glm::vec3 n;
            iss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (token == "vt") {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            texcoords.push_back(uv);
        }
        else if (token == "f")
        {
            face f;
            for (int i = 0; i < 3; i++)
            {
                std::string vert;
                iss >> vert;
                std::replace(vert.begin(), vert.end(), '/', ' ');

                std::istringstream viss(vert);
                unsigned int vi = 0, ti = 0, ni = 0;
                viss >> vi >> ti >> ni;

                f.index[i] = vi - 1; 
            }
            m_faceArray.push_back(f);
        }
    }

    m_vertexArray.clear();
    for (const auto& pos : positions)
    {
        vertex v;
        v.m_position = pos;

        m_vertexArray.push_back(v);
    }

    std::cout << "OBJ loaded: " << m_vertexArray.size() << " vertices, "
        << m_faceArray.size() << " faces.\n";

    return true;
}


ComplexObj::ComplexObj(std::string p_OBJpath)
{
	loadFromOBJ(p_OBJpath);
}


//Pour charger l'objet :  LoadFromOBJ cf. CMesh 
// il faut "nettoyer" CMesh pour créer la classe ComplexObj



RayCastResult ComplexObj::raycastTriangle(Ray& p_ray, glm::vec2 p_tInterval,
    glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    const float EPSILON = 1e-6f;
    glm::vec3 edge1 = B - A;
    glm::vec3 edge2 = C - A;
    glm::vec3 h = glm::cross(p_ray.direction, edge2);
    float a = glm::dot(edge1, h);
    if (std::abs(a) < EPSILON) return RayCastResult();

    float f = 1.0f / a;
    glm::vec3 s = p_ray.origin - A;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return RayCastResult();

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(p_ray.direction, q);
    if (v < 0.0f || u + v > 1.0f) return RayCastResult();

    float t = f * glm::dot(edge2, q);
    if (t < p_tInterval.x || t > p_tInterval.y) return RayCastResult();

    RayCastResult result;
    result.hit = true;
    result.t = t;
    result.hitPosition = p_ray.origin + t * p_ray.direction;
    result.hitNormal = glm::normalize(glm::cross(edge1, edge2));
    result.uvw.x = u;
    result.uvw.y = v;
    return result;
}

RayCastResult ComplexObj::raycast(Ray& p_ray, glm::vec2 p_tInterval)
{
    RayCastResult result;
    float closestT = p_tInterval.y;

    // Matrice monde de l'objet
    glm::mat4 modelMatrix = getModel();

    for (const face& f : m_faceArray)
    {
        // Sommets locaux
        glm::vec3 v0 = m_vertexArray[f.index[0]].m_position;
        glm::vec3 v1 = m_vertexArray[f.index[1]].m_position;
        glm::vec3 v2 = m_vertexArray[f.index[2]].m_position;

        // Passage en espace monde
        glm::vec3 p0 = glm::vec3(modelMatrix * glm::vec4(v0, 1.0f));
        glm::vec3 p1 = glm::vec3(modelMatrix * glm::vec4(v1, 1.0f));
        glm::vec3 p2 = glm::vec3(modelMatrix * glm::vec4(v2, 1.0f));

        // --- Möller–Trumbore ---
        const float EPSILON = 1e-6f;

        glm::vec3 edge1 = p1 - p0;
        glm::vec3 edge2 = p2 - p0;

        glm::vec3 h = glm::cross(p_ray.direction, edge2);
        float a = glm::dot(edge1, h);

        if (fabs(a) < EPSILON)
            continue;

        float f_inv = 1.0f / a;
        glm::vec3 s = p_ray.origin - p0;
        float u = f_inv * glm::dot(s, h);

        if (u < 0.0f || u > 1.0f)
            continue;

        glm::vec3 q = glm::cross(s, edge1);
        float v = f_inv * glm::dot(p_ray.direction, q);

        if (v < 0.0f || u + v > 1.0f)
            continue;

        float t = f_inv * glm::dot(edge2, q);

        if (t < p_tInterval.x || t > closestT)
            continue;

        // --- HIT ---
        closestT = t;

        result.hit = true;
        result.t = t;
        result.hitPosition = p_ray.origin + t * p_ray.direction;

        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        result.frontFace = glm::dot(p_ray.direction, normal) < 0.0f;
        result.hitNormal = result.frontFace ? normal : -normal;

        result.collider = this;
        result.material = getMaterialPtr();

        // barycentriques (UVW)
        result.uvw = glm::vec3(1.0f - u - v, u, v);
    }

    return result;
}

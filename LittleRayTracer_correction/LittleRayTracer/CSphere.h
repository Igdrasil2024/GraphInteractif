#pragma once
#include "Collider.h"
#include "CEntity3D.h"

class Sphere :
    public Entity3D,
    public Collider
{
public:

    Sphere();
    ~Sphere();

    // Pour modifier le rayon
    inline void setRadius(float p_radius) { m_radius = p_radius; }
    // pour récupérer le rayon
    inline float getRadius() { return m_radius; }

    // méthode de lancer de rayon pour détecter une collision entre un rayon est la sphère
    // l'intervalle de "temps" permet de limiter la recherche de collision entre 2 poits donnés
    virtual  RayCastResult raycast(Ray& p_ray, glm::vec2 p_tInterval);

protected:
    float m_radius;     // rayon de la sphère




};


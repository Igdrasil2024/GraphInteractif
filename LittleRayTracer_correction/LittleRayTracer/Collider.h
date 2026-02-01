#pragma once
#include "Utils.h"
class RayCastResult;
#include "CRayCastResult.h"
class Ray;
#include "CRay.h"

class Collider
{
public:

	// méthode pour tester la collision entre un rayon et l'objet
	// Méthode virtuelle pure : à implémenter pour chaque type d'objet (sphère, plan, maillage 3D composé de triangles ...)
	virtual RayCastResult raycast(Ray& p_ray, glm::vec2 p_tInterval) = 0;

};


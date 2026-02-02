
#pragma once
#include "Utils.h"
#include "CMaterial.h"
#include "CEntity3D.h"

// pour savoir que la classe CRay est définie (inclusion multiple)
class Ray;
#include "CRay.h"

// Pour savvoir que la classe Collider est définie (inclusion multiple)
class Collider;
#include "Collider.h"

class RayCastResult
{

public:
	bool hit = false;									// le rayon a rencontré quelque chose ? oui/non

	float t = INFINITY;									// "temps" passé avant la collision 
	glm::vec3 hitPosition = glm::vec3(0,0,0);			// position de la collision
	glm::vec3 hitNormal = glm::vec3(0,1,0);				// normale au point touché
	bool frontFace = true;								// rayon à l'intérieur ou à l'extérieur de l'objet ?
	Entity3D* collider = nullptr;						// pointeur vers le collider touché

	glm::vec3 uvw = glm::vec3(0, 0, 0);					// coordonnées UVW du point touché
	Material* material = NULL;							// matériau de l'objet touché

	// Méthode pour lancer un rayon vers les colliders de la scènes (passés en paramètre)
	static RayCastResult raycastOnColliders(Ray r, std::vector<Collider*>* p_colliders);
};


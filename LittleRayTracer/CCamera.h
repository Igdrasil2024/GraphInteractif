#pragma once
#include "Utils.h"
#include "CEntity3D.h"
#include "Collider.h"
#include "CRay.h"

#define MAX_DEPTH			5
#define EPSILON				0.00001f

// une caméra est un objet 3D dans l'espace pour lequel on adjoint des propriétés propres à la caméra
class Camera : public Entity3D
{
public:
		
	Camera();
	~Camera();

	// méthode pour initialiser la camera et le plan à travers lequel faire le rendu
	void initRenderWithResolution(glm::ivec2 p_resolution, float p_focalPlaneDistance, float p_focalPlaneWidth, std::vector<Collider*>* p_objectListPtr);

	// Méthode pour évaluer la couleur d'un pixel à l'écran
	glm::vec3 evaluatePixelColor(glm::ivec2 p_pixelCoordinates, int p_depth=MAX_DEPTH, int p_nbSamples=1);

protected:
	// Méthode pour lancer un rayon
	glm::vec3 raytrace(Ray& r, int p_depth = MAX_DEPTH, float p_currentN = 1.0f)	;

	
	float m_focalDistance;			// distance caméra/plan focal
	glm::vec2 m_focalPlaneSize;		// taille du plan focal (largeur/hauteur)

	glm::vec3 m_posFocalPlane;		// position du plan focal
	glm::vec3 m_uAxisFocalPlane;	// axe horizontal du plan focal
	glm::vec3 m_vAxisFocalPlane;	// axe vertical du plan focal
	glm::vec3 m_offsetUV;			// offset UV correspond à l'offset à appliquer pour se retrouver en 0,0
	glm::vec2 m_coefHomogeneous;	// coefficient pour passer de longueurs réelles à des valeurs homogènes (entre 0 et 1 <=> 0 et 100% dsur la largeur ou la hauteur de l'écran)

	std::vector<Collider*>* m_objectListPtr;	// pointeur sur la liste des objets de la scène

};


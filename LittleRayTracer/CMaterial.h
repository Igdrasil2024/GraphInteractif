#pragma once
#include "Utils.h"



class Material
{
public:
	Material();
	~Material();

	float lightIntensity;	// si lightIntensity >0 l'objet émet de la lumière de couleur "color"
	
	glm::vec3 color;		// couleur du matériau
	float roughness;		// "rugosité" du matériau, 0 = surface parfaite (inexistant dans la vie réelle)
	float metallic;			// coefficient entre 0 et 1 : 1 = "miroir parfait", 0 = surface mate
	float fresnelCoef;		// coefficient de fresnel : utilisé pour la réflexion ou/et la refraction
	float n;				// indice du matériau : utilisé pour la réfraction
	float transparency;
};


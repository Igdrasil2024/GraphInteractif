#include "ComplexObj.h"

//Constructeur ==> cf. CMesh
ComplexObj::ComplexObj(std::string p_OBJpath)
{
	// A remplir ... appeler LoadFromOBJ 
}


//Pour charger l'objet :  LoadFromOBJ cf. CMesh 
// il faut "nettoyer" CMesh pour créer la classe ComplexObj



RayCastResult ComplexObj::raycastTriangle(Ray& p_ray, glm::vec2 p_tInterval, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	// cf. Realtime rendering
	// https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/code/raytri_tam.pdf ==> non culling branch
}

RayCastResult ComplexObj::raycast(Ray& p_ray, glm::vec2 p_tInterval)
{

	// Boucle sur tous les triangles de mon objet
		// J'appelle pour chaque triangle raycastTriangle 
		// Je mets à jour l'interval si besoin
	// A la fin de la boucle j'ai la collision la plus proche de l'origine du rayon

	// normale à un triangle = produit vectoriel de 2 de ses côtés (dans le bon ordre)
	// UV du point touché sur le triangle (interpolation entre les 3 UVs des sommets du triangles)
}
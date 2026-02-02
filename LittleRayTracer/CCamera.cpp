#include "Utils.h"
#include "CCamera.h"
#include "CRayCastResult.h"
#include "CSphere.h"

Camera::Camera() :Entity3D(),
m_focalDistance(1.0f),
m_focalPlaneSize(1.6f,0.9f),
m_objectListPtr(NULL),
m_posFocalPlane(0,0,-1),
m_uAxisFocalPlane(1,0,0),
m_vAxisFocalPlane(0,1,0),
m_offsetUV(0,0,0),
m_coefHomogeneous(1,1)
{


}

Camera::~Camera()
{

}



void Camera::initRenderWithResolution(glm::ivec2 p_resolution, float p_focalPlaneDistance, float p_focalPlaneWidth, std::vector<Collider*>* p_objectListPtr)
{
	m_objectListPtr = p_objectListPtr;	// on pointe sur la liste des objets à tester

	// on calcule les propriétés du plan focal
	m_focalPlaneSize = glm::vec2(p_focalPlaneWidth, p_focalPlaneWidth * p_resolution.y / (float)p_resolution.x);
	m_focalDistance = p_focalPlaneDistance;
	m_posFocalPlane = model2WorldPos(glm::vec3(0, 0, -m_focalDistance));
	m_uAxisFocalPlane = glm::normalize(model2WorldDir(glm::vec3(1, 0, 0)));
	m_vAxisFocalPlane = glm::normalize(model2WorldDir(glm::vec3(0, -1, 0)));
	m_offsetUV = -(m_focalPlaneSize.x / 2.0f) * m_uAxisFocalPlane - (m_focalPlaneSize.y / 2.0f) * m_vAxisFocalPlane;
	m_coefHomogeneous = glm::vec2(m_focalPlaneSize.x / (float)p_resolution.x, m_focalPlaneSize.y / (float)p_resolution.y);
	m_uAxisFocalPlane *= m_coefHomogeneous.x;
	m_vAxisFocalPlane *= m_coefHomogeneous.y;

}




glm::vec3  Camera::raytrace(Ray& p_ray, int p_depth, float p_currentN)
{
	
	// On s'arrête si on est "trop loin" (condition d'arrêt)
	if(p_depth==0)
		return glm::vec3(0.0f);
	
	// On recherche la collision la plus proche sur tous les objets de la scène
	glm::vec2 interval(EPSILON, FLT_MAX);
	std::vector<Collider*>::iterator it = m_objectListPtr->begin();
	RayCastResult rcr;
	for (; it != m_objectListPtr->end(); it++)
	{
		Collider* c = (*it);
		RayCastResult rcrTemp = c->raycast(p_ray,interval);
		if (rcrTemp.hit)
		{
			rcr = rcrTemp;
			interval.y = rcr.t;
		}
	}

	if(!rcr.hit)
		return glm::mix(GROUND_COLOR,SKY_COLOR,glm::max(0.0f,glm::dot(glm::vec3(0,1,0), p_ray.direction)) ) ; // Lumière ambiante

	
	glm::vec3 color = glm::vec3(rcr.material->lightIntensity); // Si l'objet émet de la lumière => on émet

	// Si l'objet est réfléchissant on lance le rayon réfléchi
	// CADEAU : la réflexion est implémentée, à vous de vous en inspirer pour développer la réfraction si vous choisissez cet effet
	if (rcr.material->metallic>0.0f)
	{
		glm::vec3 normalRef = glm::normalize(rcr.hitNormal + glm::ballRand(1.0f - EPSILON) * rcr.material->roughness); // on modifie la normale en fonction de la rugosité du matériau
		Ray rReflection(rcr.hitPosition, glm::reflect(p_ray.direction, normalRef));
		color += raytrace(rReflection, p_depth - 1) * rcr.material->metallic; // On prend en compte la couleur avec le coefficient correspondant à la quantité réfléchie
	}

	glm::vec3 normal = glm::normalize(rcr.hitNormal + glm::ballRand(1.0f-EPSILON) );	// On "tire" des normales dans une demi sphère à partir du point touché pour voir quelle est la couleur qui nous éclaire (différence par rapport à ce qui a été développé en cours : on n'utilise pas la rugosité, elle est utilisé par la réflexion uniquement ici)
	Ray r(rcr.hitPosition, normal);
	color += raytrace(r, p_depth - 1) * (1-rcr.material->metallic);	// On module la couleur en fonction de la quantité réfléchie


	return color * rcr.material->color /(1.0f+glm::distance(rcr.hitPosition, p_ray.origin)); // la lumière émise par le point est "filtrée" par la couleur de l'objet et on crée une atténuation en fonction de la distance parcourue par le rayon


}


glm::vec3 Camera::evaluatePixelColor(glm::ivec2 p_pixelCoordinates, int p_depth, int p_nbSamples)
{
	
	// on calcule le point à travers lequel doit passer le rayon en fonction des coordonnées passées en paramètres
	// le décalage de 0.5 est pour se positionner sur le centre du pixel
	glm::vec3 pixelPosInWorld = m_posFocalPlane + m_offsetUV
									+ (p_pixelCoordinates.x + 0.5f) * m_uAxisFocalPlane 
									+ (p_pixelCoordinates.y + 0.5f) * m_vAxisFocalPlane;
	
	// Anti alisiasing : on tire un point aléatoirement dans la surface du pixel 
	pixelPosInWorld += glm::linearRand(-0.5f, 0.5f) * m_uAxisFocalPlane;
	pixelPosInWorld += glm::linearRand(-0.5f, 0.5f) * m_vAxisFocalPlane;

	glm::vec3 dir = glm::normalize(pixelPosInWorld - m_position);
	
	//Ray r(m_position,dir);
	Ray r;
	r.origin = m_position;
	r.direction = dir;

	// On calcule le rayon à partir de la caméra vers le point
	// On lance un rayon (raytrace) à partir de la caméra et on renvoie le résultat
	return raytrace(r, p_depth);
	//return glm::vec3(1,1,0);

}
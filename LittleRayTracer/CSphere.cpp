#include "Utils.h"
#include "CSphere.h"


Sphere::Sphere() : Entity3D(), Collider(),
	m_radius(1.0f)
{
}

Sphere::~Sphere()
{

}

RayCastResult Sphere::raycast(Ray& p_ray, glm::vec2 p_tInterval)
{

	RayCastResult rcr;

	//b = 2.dir.CO
	//c = CO.CO -r²
	// delta = b²-4c
	glm::vec3 CO = p_ray.origin - m_position;
	float b = 2.0f * glm::dot(p_ray.direction, CO);
	float c = glm::dot(CO, CO) - m_radius * m_radius;
	float delta = b * b - 4 * c;
		
	if (delta == 0)
	{
		float t0 = -b / 2.0f;
		if (t0 >= 0)
		{
			rcr.t = t0;
			rcr.hit = true;
		}
	}
	else if (delta > 0)
	{
		float t1 = (-b - sqrt(delta)) / 2.0f;
		float t2 = (-b + sqrt(delta)) / 2.0f;
		if (t1 < 0 && t2>=0)
		{
			rcr.t = t2;
			rcr.hit = true;
			rcr.frontFace = false;
		}
		else if (t1 >= 0 )
		{
			rcr.t = t1;
			rcr.hit = true;
		}
	}

	if (rcr.hit && (rcr.t< p_tInterval.x || rcr.t > p_tInterval.y))
	{
		rcr.hit = false;
		//return rcr;
	}

	if (rcr.hit)
	{
		rcr.hitPosition = p_ray.origin + rcr.t * p_ray.direction;
		rcr.hitNormal = glm::normalize(rcr.hitPosition - m_position);
		rcr.collider = this;
		rcr.material = &m_material;
		rcr.uvw = glm::vec3(0, 0, 0); // A modifier si application d'une texture sur l'objet
	}


	// calcul de l'intersection rayon / sphere
	return rcr;

}

#pragma once



class Ray
{
public:
	Ray() : origin(glm::vec3(0, 0, 0)), direction(glm::vec3(0,0,-1)) {}
	Ray(glm::vec3 p_origin, glm::vec3 p_direction) : origin(p_origin), direction(p_direction) {}
	~Ray() {}

	glm::vec3 origin;		// origine du rayon
	glm::vec3 direction;	// direction du rayon (logiquement elle est normalisée)

};


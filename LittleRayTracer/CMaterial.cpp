#include "Utils.h"
#include "CMaterial.h"


Material::Material()
{
	lightIntensity = 0.0f;
	color = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));
	roughness = glm::linearRand(0.0f, 1.0f);
	metallic = glm::linearRand(0.001f, 1.0f);
	fresnelCoef = 0.0f;
	n = 1.0f;
}

Material::~Material()
{

}
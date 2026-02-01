#pragma once
#include "CMaterial.h"


// pour définir un objet 3D dans l'espace : sa position, son orientation ...
class Entity3D
{
public:
	Entity3D();
	~Entity3D();

	// Pour récupérer la position de l'objet
	inline glm::vec3 getPosition() { return m_position; }
	// pour récupérer l'ortientation de l'objet
	inline glm::quat getOrientation () { return m_orientation; }

	// pour modifier la position de l'objet
	inline void setPosition(glm::vec3 p_position) { m_position =  p_position; UpdateModel(); }
	// pour modifier l'orientation de l'objet
	inline void setOrientation(glm::vec3 p_eulerAngles) { m_orientation = glm::quat(p_eulerAngles);  UpdateModel(); }

	// pour récupérer la matrice MODEL
	inline glm::mat4 getModel() { return m_model; }

	// Pour convertir une position du référentiel de l'objet vers le référentiel du monde
	inline glm::vec3 model2WorldPos(glm::vec3 p_modelPos) { return (m_model * glm::vec4(p_modelPos, 1.0f)).xyz; }
	// Pour convertir une direction du référentiel de l'objet vers le référentiel du monde
	inline glm::vec3 model2WorldDir(glm::vec3 p_modelDir) { return (m_model * glm::vec4(p_modelDir, 0.0f)).xyz; }
	// Pour convertir une position du référentiel monde vers le référentiel de l'objet 
	inline glm::vec3 world2ModelPos(glm::vec3 p_worldPos) { return (m_modelInverse * glm::vec4(p_worldPos, 1.0f)).xyz; }
	// Pour convertir une direction du référentiel monde vers le référentiel de l'objet 
	inline glm::vec3 world2ModelDir(glm::vec3 p_worldDir) { return (m_modelInverse * glm::vec4(p_worldDir, 0.0f)).xyz; }
	// pour récupérer le matériau de l'objet
	inline Material* getMaterialPtr() { return &m_material; }


protected:

	// Pour mettre à jour les matrices correspondantes à la position/orientation de l'objet
	inline void UpdateModel() { 
		m_model = glm::translate(glm::rotate(glm::mat4(1.0f), glm::angle(m_orientation), glm::axis(m_orientation)), m_position);
		m_modelInverse = glm::inverse(m_model);
	}
		
	glm::vec3 m_position;			// position de l'objet dans le monde
	glm::quat m_orientation;		// orientation de l'objet dans le monde
	
	glm::mat4 m_model;				// matrice model (objet => monde)
	glm::mat4 m_modelInverse;		// matrice inverse de la matrice model (monde => objet)

	Material m_material;			// matériau de l'objet
};


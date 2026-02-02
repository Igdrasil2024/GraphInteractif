#pragma once
#include "Utils.h"
#include "CCamera.h"
#include "Collider.h"

class LittleRaytracer
{
public:
	LittleRaytracer(glm::ivec2 p_outputRes); 
	~LittleRaytracer();

	// Méthode qui lance le rendu (boucle qui rend pixel par pixel)
	void run();

protected:
	// initialisation de la fenêtre
	int init();
	// Mise à jour de la couleur d'un pixel dans le buffer utilisée par la fenêtre pour l'affichage
	// Mise à a jour du rendu dans la fenêtre si p_present = true
	void updatePixelOnScreen(int p_x, int p_y, glm::vec4 p_rgbAcc, bool p_present = false);

	// to save a BMP file of the current screen
	void save();

	// Mise à jour de la fenêtre complète : plus rapide que pixel par pixel
	void updateScreen();

	bool m_running;					// pour savoir si on veut quitter ou non la boucle 

	SDL_Window* m_window;			// pointeur sur la fenêtre SDL (SDL : librairie qui permet de créer et gérer facilement les fenêtres)
	SDL_Renderer* m_renderer;		// outil de rendu dans la fenêtre (permet de modifier la couleur des pixels un par un par exemple)
	SDL_Texture* m_screenTexture;	// zone de rendu de la fenêtre stockée sous forme de texture

	glm::ivec2 m_resolution;		// résolution de la fenêtre
	float m_gamma;					// valeur de gamma

	glm::vec4* m_pixelsAcc;			// tableau contenant l'accumulation des passes de rendu
	uint32_t* m_pixels;				// tableau contenant les pixels à afficher dans la fenêtre

	Camera* m_camera;				// pointeur vers la caméra
	
	std::vector<Collider*> m_objectList;	// liste des objets dans la scène


};


#include "Utils.h"
#include "CLittleRaytracer.h"



int main(int argc, char** argv)
{
	// Résolution de la fenêtre : 640x480 par défaut
	glm::ivec2 res = glm::ivec2(640, 480);
	if (argc > 2)
	{
		// Peut être modifiée via les paramètres en ligne de commande
		res.x = atoi(argv[1]);
		res.y = atoi(argv[2]);
	}

	// On crée le "raytracer" (et la fenêtre en même temps)
	LittleRaytracer lrt(res);

	// On lance le rendu 
	// on quitte lorsque le rendu est satisfaisant en fermant la fenêtre : ESC ou croix en haut à droite dans la barre de titre)
	lrt.run();

	return 0;
}
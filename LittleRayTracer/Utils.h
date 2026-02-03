#pragma once

// SDL : pour gérer la fenêtre et le rendu
#include "./ThirdPartyLibs/SDL2-2.0.18/include/SDL.h"

// GLM : pour faciliter les calculs vectoriels 
#define GLM_FORCE_SWIZZLE
#include "./ThirdPartyLibs/glm/glm/glm.hpp"
#include "./ThirdPartyLibs/glm/glm/ext.hpp"

// librairies standard
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <omp.h>

#define SKY_COLOR			glm::vec3(1.8f,2.0f,2.0f)
#define GROUND_COLOR		glm::vec3(0.0f,0.0f,0.1f)
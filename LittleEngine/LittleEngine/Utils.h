/// \file	Utils.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#ifndef _UTILS_
#define _UTILS_

// GLEW 
#include "../thirdPartyLibs/glew-2.1.0/include/GL/glew.h"

// SDL 
#ifdef WIN32
	#include <windows.h>
	#include "../../LittleRayTracer/ThirdPartyLibs/SDL2-2.0.18/include/SDL_syswm.h"
#endif
#include "../../LittleRayTracer/ThirdPartyLibs/SDL2-2.0.18/include/SDL.h"

#include "../../LittleRayTracer/ThirdPartyLibs/SDL2-2.0.18/include/SDL_opengl.h"


// GLM 
#define GLM_FORCE_SWIZZLE
#include "../../LittleRayTracer/ThirdPartyLibs/glm/glm/glm.hpp"
#include "../../LittleRayTracer/ThirdPartyLibs/glm/glm/gtc/type_ptr.hpp"
#include "../../LittleRayTracer/ThirdPartyLibs/glm/glm/detail/_swizzle.hpp"
#include "../../LittleRayTracer/ThirdPartyLibs/glm/glm/gtc/matrix_transform.hpp"
#include "../../LittleRayTracer/ThirdPartyLibs/glm/glm/gtc/matrix_access.hpp"
#include "../../LittleRayTracer/ThirdPartyLibs/glm/glm/gtc/matrix_inverse.hpp"
#include "../../LittleRayTracer/ThirdPartyLibs/glm/glm/gtc/quaternion.hpp"

// STD
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <string>

// Conversion constants
#define DEG2RAD	0.01745329251994329576923690768489
#define RAD2DEG	57.295779513082320876798154814105

// To define mouse button numbers
#define LEFT	0
#define	RIGHT	1
#define CENTER	2


#endif
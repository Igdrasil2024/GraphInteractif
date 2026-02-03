#include "Utils.h"
#include "CLittleRaytracer.h"
#include "CSphere.h"
#include "ComplexObj.h"
#include <omp.h>
#include "ThirdPartyLibs/SDL2-2.0.18/include/SDL.h"

LittleRaytracer::LittleRaytracer(glm::ivec2 p_outputRes) :
	m_window(NULL),
	m_renderer(NULL),
	m_screenTexture(NULL),
	m_resolution(p_outputRes),
	m_camera(NULL),
	m_gamma(1.0f),
	m_pixels(NULL),
	m_running(false),
	m_pixelsAcc(nullptr)
{
}


LittleRaytracer::~LittleRaytracer()
{

	if (m_pixels)
		delete[] m_pixels;
	if (m_pixelsAcc)
		delete[] m_pixelsAcc;

	if (m_screenTexture)
		SDL_DestroyTexture(m_screenTexture);

	delete m_camera;

	for (int i = 0; i < m_objectList.size(); i++)
		delete m_objectList[i];

	if (m_renderer)
		SDL_DestroyRenderer(m_renderer);
	if (m_window)
		SDL_DestroyWindow(m_window);
	SDL_Quit();
}




int LittleRaytracer::init()
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	SDL_CreateWindowAndRenderer(m_resolution.x, m_resolution.y, 0, &m_window, &m_renderer);
	SDL_SetWindowTitle(m_window, "FOKS lab (c) littleRayTracer");
	SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 0);
	SDL_RenderClear(m_renderer);
	SDL_RenderPresent(m_renderer);

	m_camera = new Camera();
	/*

	{
		
		Sphere* sphere = new Sphere();
		sphere->setPosition(glm::vec3(-0.25f, 0.f, -1.8f));
		sphere->setRadius(0.25f);
		sphere->getMaterialPtr()->color = glm::vec3(0.3, 0.05, 0.1);
		sphere->getMaterialPtr()->roughness = 0.1f;
		sphere->getMaterialPtr()->metallic = 0.5f;
		sphere->getMaterialPtr()->lightIntensity = 10.0f;

		m_objectList.push_back(sphere);
	}
	
	{
		Sphere* sphere = new Sphere();
		sphere->setPosition(glm::vec3(0.2f, 0.f, -2.1f));
		sphere->setRadius(0.25);
		sphere->getMaterialPtr()->color = glm::vec3(0.5f, 0.7f, 0.4f);
		sphere->getMaterialPtr()->roughness = 0.0f;
		sphere->getMaterialPtr()->metallic = 1.0f;
		sphere->getMaterialPtr()->lightIntensity = 0.f;
		m_objectList.push_back(sphere);
	}
	*/
	{
		Sphere* sphere = new Sphere();
		sphere->setPosition(glm::vec3(-0.25f, -50.25, -2.2f));
		sphere->setRadius(50.0);
		sphere->getMaterialPtr()->color = glm::vec3(0.5f);
		sphere->getMaterialPtr()->roughness = 0.05f;
		sphere->getMaterialPtr()->metallic = 0.8f;
		sphere->getMaterialPtr()->lightIntensity = 0.f;
		m_objectList.push_back(sphere);
	}

	{
		Sphere* sphere = new Sphere();
		sphere->setPosition(glm::vec3(0.75f, 0.75f, -1.5f));
		sphere->setRadius(0.5f);
		sphere->getMaterialPtr()->color = glm::vec3(1, 0.5, 0.2);
		sphere->getMaterialPtr()->roughness = 0.0f;
		sphere->getMaterialPtr()->metallic = 0.f;
		sphere->getMaterialPtr()->lightIntensity = 200.0f;
		m_objectList.push_back(sphere);
	}

	{
		ComplexObj* obj = new ComplexObj("Triangle.obj");

		obj->setPosition(glm::vec3(-1.25f, 0.0f, -5.0f));
			
		obj->getMaterialPtr()->color = glm::vec3(0.3, 0.05, 0.1);
		obj->getMaterialPtr()->roughness = 1.0f;
		obj->getMaterialPtr()->metallic = 1.0f;
		obj->getMaterialPtr()->lightIntensity = 10.0f;
		m_objectList.push_back(obj);
	}
	





	// create texture
	m_screenTexture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		m_resolution.x,
		m_resolution.y);

	if (m_screenTexture == NULL) {
		SDL_Log("Unable to create texture: %s", SDL_GetError());
		return 1;
	}



	m_pixels = new uint32_t[m_resolution.x * m_resolution.y];
	m_pixelsAcc = new glm::vec4[m_resolution.x * m_resolution.y];

	memset(m_pixels, 0, m_resolution.x * m_resolution.y);
	memset(m_pixelsAcc, 0, m_resolution.x * m_resolution.y * sizeof(glm::vec4));


	return 0;
}



void LittleRaytracer::updatePixelOnScreen(int p_x, int p_y, glm::vec4 p_rgbAcc, bool p_present)
{
	if (p_rgbAcc.w == 0.0f)
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	else
	{

		glm::vec3 color = glm::clamp(p_rgbAcc.rgb() / p_rgbAcc.w, glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

		glm::ivec4 RGBA(0, 0, 0, 255);
		for (int i = 0; i < 3; i++)
			RGBA[i] = glm::clamp((int)(glm::pow(glm::clamp(color[i], 0.0f, 1.0f), 1.0f / m_gamma) * 256.0f), 0, 255);

		SDL_SetRenderDrawColor(m_renderer, RGBA.r, RGBA.g, RGBA.b, RGBA.a);
	}
	SDL_RenderDrawPoint(m_renderer, p_x, p_y);

	if (p_present)
		SDL_RenderPresent(m_renderer);
}


void LittleRaytracer::updateScreen()
{
#pragma omp parallel for schedule(dynamic)
	for (int x = 0; x < m_resolution.x; x++)
	{
		for (int y = 0; y < m_resolution.y; y++)
		{
			glm::vec4 pixelAcc = m_pixelsAcc[y * m_resolution.x + x];

			if (pixelAcc.w == 0.0f)
				m_pixels[y * m_resolution.x + x] = 0;
			else
			{
				glm::vec3 c = pixelAcc.xyz() / pixelAcc.w;
				for (int i = 0; i < 3; i++)
					c[i] = glm::pow(glm::clamp(c[i], 0.0f, 1.0f), 1.0f / m_gamma);

				m_pixels[y * m_resolution.x + x] = ((unsigned char)(c.b * 255.99) << 16) + ((unsigned char)(c.g * 255.99) << 8) + ((unsigned char)(c.r * 255.99));
			}
		}
	}

	int texture_pitch = 0;
	void* texture_pixels = NULL;
	if (SDL_LockTexture(m_screenTexture, NULL, &texture_pixels, &texture_pitch) != 0)
	{
		SDL_Log("Unable to lock texture: %s", SDL_GetError());
	}
	else
	{
		memcpy(texture_pixels, m_pixels, texture_pitch * m_resolution.y);
	
	}
	SDL_UnlockTexture(m_screenTexture);
	SDL_RenderCopy(m_renderer, m_screenTexture, NULL, NULL);
	SDL_RenderPresent(m_renderer);

	SDL_Delay(0);
}

void LittleRaytracer::save()
{
	SDL_Surface* sshot = SDL_CreateRGBSurface(0, m_resolution.x, m_resolution.y, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(m_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	std::string screenshotName = "screenshot_";
	int screenshotIndex = 0;
	while (std::filesystem::exists(screenshotName + std::to_string(screenshotIndex) + ".bmp"))
		screenshotIndex++;
	SDL_SaveBMP(sshot, (screenshotName + std::to_string(screenshotIndex) + ".bmp").c_str());
	SDL_FreeSurface(sshot);
}

void LittleRaytracer::run()
{
	m_running = init() == 0;

	if (m_camera)
		m_camera->initRenderWithResolution(m_resolution, 1, 1, &m_objectList);


	glm::ivec2 currentPixelCoordinates = glm::vec2(0, 0);

	int numFrame = 1;

	SDL_Event event;
	while (m_running) {


		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				// Quit
				m_running = false;
				break;
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					// quit
					m_running = false;
					break;
				case SDLK_SPACE:
				{
					save();
				}
				break;
				case SDLK_RETURN:
					// On recommence depuis le début
					memset(m_pixels, 0, m_resolution.x * m_resolution.y * sizeof(uint32_t));
					memset(m_pixelsAcc, 0, m_resolution.x * m_resolution.y * sizeof(glm::vec4));

					currentPixelCoordinates = glm::vec2(0, 0);
					numFrame = 1;
					updateScreen();
					break;
				case SDLK_UP:
					m_gamma += 0.1f;
					m_gamma = glm::min(5.0f, m_gamma);
					break;
				case SDLK_DOWN:
					m_gamma -= 0.1f;
					m_gamma = glm::max(0.1f, m_gamma);
					break;

				}

			}
			}
		}




		// on rend un pixel 
		//glm::vec3 color = glm::ivec3(glm::linearRand(0, 1), glm::linearRand(0, 1), glm::linearRand(0, 1));
		glm::vec3 color = m_camera->evaluatePixelColor(currentPixelCoordinates, 4);
		m_pixelsAcc[currentPixelCoordinates.y * m_resolution.x + currentPixelCoordinates.x] += glm::vec4(color, 1);

		// on se décale d'un pixel à droite
		currentPixelCoordinates.x++;
		// Si on est au bout de la ligne on passe à la ligne
		if (currentPixelCoordinates.x == m_resolution.x)
		{
			currentPixelCoordinates.x = 0;
			currentPixelCoordinates.y++;

			// si on est en bas de l'image, on l'affiche et on revient au début
			if (currentPixelCoordinates.y >= m_resolution.y)
			{
				std::string title = "FL(c) [" + std::to_string(numFrame) + "][" + std::to_string((int)(m_gamma * 10)) + "]";
				SDL_SetWindowTitle(m_window, title.c_str());
				updateScreen();
				currentPixelCoordinates = glm::vec2(0, 0);
				numFrame++;
			}
		}

	}
}


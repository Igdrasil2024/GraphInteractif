/// \file	Application.cpp
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024
/// 

#include "stdafx.h"
#include "Application.h"


// init the static memeber variable to manage the singleton
CApplication * CApplication::m_instance = nullptr;


CApplication::CApplication() :
	m_window(nullptr),
	m_glContext(nullptr),
	m_res(glm::ivec2(640, 480)),
	m_quit(false),
	m_mouseWheel(0),
	m_model(glm::mat4(1.0)),
	m_view(glm::mat4(1.0)),
	m_projection(glm::mat4(1.0)),
	m_camera(nullptr),
	m_ctrlKeys{ false,false },
	m_shiftKeys{false,false},
	m_mouseButton{false,false,false}
{
	m_camera = new CCamera("Camera");
}


CApplication::~CApplication()
{
}


CApplication * CApplication::createInstance()
{
	if (m_instance == nullptr)
		m_instance = new CApplication();
	
	return m_instance;
}




void CApplication::onKeyDown(SDL_KeyboardEvent &p_e)
{
	switch (p_e.keysym.sym)
	{
	case SDLK_ESCAPE:
		m_quit = true;	// quit the application at the end of the render loop
		break;
	case SDLK_LCTRL:
		m_ctrlKeys[LEFT] = true;
		break;
	case SDLK_RCTRL:
		m_ctrlKeys[RIGHT] = true;
		break;
	case SDLK_LSHIFT:
		m_shiftKeys[LEFT] = true;
		break;
	case SDLK_RSHIFT:
		m_shiftKeys[RIGHT] = true;
		break;
	case SDLK_p:
		glPolygonMode(GL_FRONT, GL_POINT);
		glPolygonMode(GL_BACK, GL_POINT);
		break;
	case SDLK_w:
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_LINE);
		break;
	case SDLK_f:
		glPolygonMode(GL_FRONT, GL_FILL);
		glPolygonMode(GL_BACK, GL_FILL);
		break;
	}
}


void CApplication::onKeyUp(SDL_KeyboardEvent &p_e)
{
	switch (p_e.keysym.sym)
	{
	case SDLK_LCTRL:
		m_ctrlKeys[LEFT] = false;
		break;
	case SDLK_RCTRL:
		m_ctrlKeys[RIGHT] = false;
		break;
	case SDLK_LSHIFT:
		m_shiftKeys[LEFT] = false;
		break;
	case SDLK_RSHIFT:
		m_shiftKeys[RIGHT] = false;
		break;
	}
}


void CApplication::onMouseButtonDown(SDL_MouseButtonEvent &p_e)
{
	switch (p_e.button)
	{
	case SDL_BUTTON_LEFT:
		m_mouseButton[LEFT] = true;
		break;
	case SDL_BUTTON_RIGHT:
		m_mouseButton[RIGHT] = true;
		break;
	case SDL_BUTTON_MIDDLE:
		m_mouseButton[CENTER] = true;
		break;
	}
}

void CApplication::onMouseButtonUp(SDL_MouseButtonEvent &p_e)
{
	switch (p_e.button)
	{
	case SDL_BUTTON_LEFT:
		m_mouseButton[LEFT] = false;
		break;
	case SDL_BUTTON_RIGHT:
		m_mouseButton[RIGHT] = false;
		break;
	case SDL_BUTTON_MIDDLE:
		m_mouseButton[CENTER] = false;
		break;
	}

}

glm::vec2 angles = glm::vec2(0, 0);
float distance = 0.5f;


void CApplication::applyCameraNavigation()
{

	glm::mat4 transform = glm::rotate(glm::mat4(1), angles.y, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), angles.x, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1), glm::vec3(0, 0, distance));

	m_camera->setTransform(transform);

}

void CApplication::onMouseMotion(SDL_MouseMotionEvent &p_e)
{

	glm::mat4 t;

	angles.x -= p_e.yrel/5000.0f;
	angles.x = glm::clamp(angles.x, -glm::pi<float>() /2.0f, glm::pi<float>() / 2.0f);

	angles.y -= p_e.xrel/1000.0f;
	while (angles.y < 0)
		angles.y += 2.0f*glm::pi<float>();
	while (angles.y > 2.0f*glm::pi<float>())
		angles.y -= 2.0f*glm::pi<float>();

	applyCameraNavigation();
}

void CApplication::onMouseWheel(SDL_MouseWheelEvent &p_e)
{
	distance += glm::sign(p_e.y)*0.1f;
	distance = glm::clamp(distance, 0.1f, 10.0f);

	applyCameraNavigation();
}



void CApplication::onEvent(SDL_Event &e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		onKeyDown(e.key);
		break;
	case SDL_KEYUP:
		onKeyUp(e.key);
		break;
	case SDL_MOUSEBUTTONDOWN:
		onMouseButtonDown(e.button);
		break;
	case SDL_MOUSEBUTTONUP:
		onMouseButtonUp(e.button);
		break;
	case SDL_MOUSEMOTION:
		onMouseMotion(e.motion);
		break;
	case SDL_MOUSEWHEEL:
		onMouseWheel(e.wheel);
		break;
	case SDL_QUIT:
		m_quit = true;					// quit the application at the end of the current render loop
		break;

	}
}




bool CApplication::init()
{

// SDL =========================================================================


	if (SDL_Init(SDL_INIT_VIDEO ) < 0) // Initialize SDL's Video subsystem 
	{
		std::cout << "Unable to initialize SDL\n";
		return false;
	} // Or kill on error 


	// Request opengl context (defined by major & minor version at the beginning of this file).
	// SDL doesn't have the ability to choose which profile at this time of writing,
	// but it should default to the core profile 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// if core profile asked , set the corresponding flag 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY /*SDL_GL_CONTEXT_PROFILE_CORE*/); // Core profile not used now, (next year when implementing the shaders)

	// Double buffering ON
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// 24bits  Depth buffer (2^24 levels of depth to resolve the display of a fragment)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	// Create our window centered with the resolution of the screen (fullscreen in Release mode, half the resolution in Debug mode) 
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS;

	// get resolution of the screen  
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode( 0, &mode);			// 0 = first screen if display is extended on mulitple screens
	m_res = glm::ivec2(mode.w, mode.h);

#ifdef _DEBUG
	m_res.x /= 2;
#endif
	m_camera->setProjection((float)M_PI / 2.0f, m_res.x / (float)m_res.y);

	// Create the window centered with the wanted resolution (w/2,h) and with the parameters set in flags 
	m_window = SDL_CreateWindow("FOKS LAB (c) 2025", 0 , 0, m_res.x , m_res.y, flags);	// Our window handle
	if (!m_window) 
	{
		std::cout << "Unable to create window\n";
		return false;
	} // Kill if creation failed


	// to get mouse events all the time (no way to get out of the window)
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(0);


	// Create our opengl context and attach it to our window 
	m_glContext = SDL_GL_CreateContext(m_window);																	// Our opengl context handle 
	if (!m_glContext)
	{
		std::cout << "Unable to create OpenGL context\n";
		return false;
	}



	// This makes our buffer swap synchronized with the monitor's vertical refresh : THIS MUST BE DONE AFTER WINDOW CONTEXT CREATION 
	// 0 : render as quick as possible (tearing effect)
	// 1 : render every "screen frame" if rendering is quick enough (often 60Hz on LCD), else render into the next "screen frame" (so framerate can be 60Hz, 30Hz, 15Hz, 7.5Hz, ...)
	// -1 : render every "sreen frame" if rendering is quick enough, else render as quick as possible. So framerate is only limited by the screen refresh (framerate can be 60Hz or less)
	SDL_GL_SetSwapInterval(1);
	
	// init GLEW to use OpenGL extension (for functions of OpenGL version > 1.2 )
	glewExperimental = GL_TRUE;		// try to load the extension even if the extension is not in the driver description
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cout << "Error while initializing GLEW : %s\n";
		std::cout << glewGetErrorString(status);
		return false;
	} // no extensions ==> kill the application


// OPENGL ==================================================================================

	// FOKS LAB ---

	// Choose buffer where you will draw 
	// glDrawBuffer
	glDrawBuffer(GL_BACK);
	// set the color with which we clear the screen 
	// glClearColor
	glClearColor(0.3f, 0.3f, 0.3f, 0);
	// set the depth with which we clear the depth buffer (maximum depth)
	// glClearDepth
	glClearDepth(1);
	// // to activate some OpenGL states : glEnable
	// // to deactivate them : glDisable
	// Activate the depth test so we can draw triangles without sorting them from back to front
	// GL_DEPTH_TEST
	glEnable(GL_DEPTH_TEST);
	// Object with a depth less or equal to the depth present in the depth buffer will overwrite the depth buffer and will be displayed
	// those who are greater are discarded (at the end of the fragment shader while resolving the fragment)
	// glDepthFunc GL_EQUAL / GL_NOTEQUAL / GL_LESS / GL_LEQUAL /  / GL_GREATER / GL_GEQUAL / GL_NEVER / GL_ALWAYS 
	glDepthFunc(GL_LEQUAL);
	// Activate the lighting (switch on the "electricity meter" of the OpenGL context) - COMPATIBILITY PROFILE ONLY
	// GL_LIGHTING
	glEnable(GL_LIGHTING);
	// Activate the first light (switch on the light, can be switched off using glDisbale(GL_LIGHT0) even if lighting is switched on)
	// if you switch off the electricity meter all lights will be deactivated but can be reactivated switching it on
	// GL_LIGHT0 / GL_LIGHT1 / ...
	glEnable(GL_LIGHT0);
	// Do not display the back of the faces
	// GL_CULL_FACE
	// Choose which is the back face 
	// glCullFace GL_BACK / GL_FRONT / GL_BOTH / GL_NONE
	glCullFace(GL_BACK);
	// Can use textures on objects
	// GL_TEXTURE_2D
	glEnable(GL_TEXTURE_2D);
	// set the viewport in the screen to render into
	// glViewport
	glViewport(0, 0, m_res.x, m_res.y);
	// We can scissor / cut the window to the viewport so the only pixels modified are those in the viewport (useful if you are rendering multiple viewports on the same screen)
	// glScissor
	// GL_SCISSOR_TEST
	// Not really useful for the current application

	// 
	// COMPATIBILITY PROFILE ==> set the projection and modelview matrices
	// glMatrixMode GL_PROJECTION / GL_MODELVIEW
	// glLoadMatrixf

	glMatrixMode(GL_PROJECTION);
	m_projection = m_camera->getProjection();
	glLoadMatrixf(&m_projection[0][0]);

	glMatrixMode(GL_MODELVIEW);
	m_model = glm::mat4(1.0f); // identity
	m_view = m_camera->getView();
	glm::mat4 mv = m_view * m_model;
	glLoadMatrixf(&mv[0][0]);


// LOAD THE OBJECT FROM OBJ FILE =================================================================


	C3DObject* object = new C3DObject("FOKS");
	if (object)
	{
		CMesh* m = CMesh::loadFromOBJ("./Ressources/FOKS/FOKS.OBJ");
		if (m)
			object->addMesh(m);

		CTexture* diffuse = CTexture::loadFromBMP("./Ressources/FOKS/diffuse.BMP");
 		if (diffuse)
			m->setTexture(diffuse);

		m_objects.push_back(object);
	}

	applyCameraNavigation();

	return true;
}

void CApplication::quit()
{
	// delete all the objects in memory (easy way ... but not the best in my point off view)
	C3DNode::deleteAllNodes();
	CMesh::deleteAllMeshes();


	// Delete our opengl context, destroy our window, and shutdown SDL 

	if (m_glContext)
		SDL_GL_DeleteContext(m_glContext);
	if (m_window)
		SDL_DestroyWindow(m_window);

	// clean SDL stuff
	SDL_Quit();
}




void CApplication::interactions()
{
	// Get all the events and process them if needed
	SDL_Event e;
	while (SDL_PollEvent(&e))
		onEvent(e);

	// Add there the instructions you want to do just after processing the events ------------------------------------


}

void CApplication::render()
{

	// FOKS LAB ---
	// Clear the buffers where we draw
	// glClear GL_COLOR_BUFFER_BIT  GL_DEPTH_BUFFER_BIT
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get the view & projection matrices
	m_model = glm::mat4(1.0f);																		// Create the model matrix
	// get the view & projection matrix from the camera 
	m_view = m_camera->getView();
	m_projection = m_camera->getProjection();

	// Update the PROJECTION & MODELVIEW MATRICES (COMPATIBILITY PROFILE)
	// glMatrixMode / glLoadMatrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&m_projection[0][0]);

	glMatrixMode(GL_MODELVIEW);
	glm::mat4 mv = m_view * m_model;
	glLoadMatrixf(&mv[0][0]);

	// Draw all the objects using the view & projection (and identity for the world model matrix)
	std::vector<C3DObject*>::iterator it = m_objects.begin();
	for (; it != m_objects.end(); it++)
		(*it)->draw(m_model, m_view, m_projection);
	
	// swap the draw buffer from BACK to FRONT (double buffering configuration created when creating the window)
	SDL_GL_SwapWindow(m_window);

}


void CApplication::run()
{
	if (!init())			// create the window & initialize the OpenGL states. Load the Object to be rendered
		m_quit = true;

	// RENDER LOOP	============================================================================================================
	m_timer.start();
	while (!m_quit)
	{
		m_timer.tick();

		interactions();		// manage all the interactions (modifying the model matrix for instance)
		
		render();			// render the object

		SDL_Delay(0);		// wait "0 milliseconds" to let a process with higher priority handle the processor ressources
	}
	// =========================================================================================================================
	
	quit();					// deallocate memory before quitting
}
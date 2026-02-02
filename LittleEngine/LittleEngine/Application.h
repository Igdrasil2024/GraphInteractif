/// \file	Application.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024
/// 


#ifndef _APPLICATION_
#define _APPLICATION_

#include "stdafx.h"
#include "C3DObject.h"
#include "Camera.h"
#include "Timer.h"


///	\class	CApplication
///	\brief	to manage all the application (loading/interacting/rendering...)
///
/// this class is a singleton since only one instance of the class can be created in the program
///

class CApplication
{
protected:
	/// \brief	Constructor, cannot be called from outside to set the class as singleton
	///
	/// the instance "m_instance" is created first time the constructor is called
	CApplication();

public:
	/// \ brief	destructor
	~CApplication();
	/// \brief	static method to create the instance of the application, if call twice the second call will return the already created application pointer
	///
	///	\return	the pointer to the application instance
	///
	static CApplication * createInstance();

	/// \brief to manage the keyboard event (key down)
	/// 
	/// \param[in]	p_e	the event to manage
	/// 
	void onKeyDown(SDL_KeyboardEvent &p_e);
	/// \brief to manage the keyboard event (key up)
	/// 
	/// \param[in]	p_e	the event to manage
	///
	void onKeyUp(SDL_KeyboardEvent &p_e);
	/// \brief to manage the mouse wheel event
	/// 
	/// \param[in]	p_e	the event to manage
	///
	void onMouseWheel(SDL_MouseWheelEvent &p_e);
	/// \brief to manage the mouse motion event
	/// 
	/// \param[in]	p_e	the event to manage
	///
	void onMouseMotion(SDL_MouseMotionEvent &p_e);
	/// \brief to manage the mouse button event (button down)
	/// 
	/// \param[in]	p_e	the event to manage
	///
	void onMouseButtonDown(SDL_MouseButtonEvent &p_e);
	/// \brief to manage the mouse button event (button up)
	/// 
	/// \param[in]	p_e	the event to manage
	/// 	
	void onMouseButtonUp(SDL_MouseButtonEvent &p_e);
	/// \brief	to manage all the event and translate them to the specific function to manage them
	/// 
	/// \param[in]	p_e	the event to manage
	/// 
	void onEvent(SDL_Event &p_e);
	
	/// \brief to process the renderloop
	void run();


	
protected:
	/// \brief to initialize the SDL window and the OpenGL states
	bool init();
	/// \brief to delete all the data allocated by the application before quitting (called when m_quit becomes true)
	void quit();

	/// \brief to manage all the interactions (keyboard, mouse, joypad, ...)
	void interactions();

	/// \brief	to process the rendering
	void render(); 

	void applyCameraNavigation();


	SDL_Window *	m_window;				///< to store the window pointer
	glm::ivec2		m_res;
	SDL_GLContext	m_glContext;			///< to store the OpenGL context of the window


	bool		m_quit;						///< to know when we will have to quit

	bool		m_mouseButton[3];			///< states of the mouse buttons
	int			m_mouseWheel;				///< value of the mouse wheel
	bool		m_ctrlKeys[2];				///< states of the 2 CTRL keys 
	bool		m_shiftKeys[2];				///< states of the 2 SHIFT keys

	glm::mat4	m_model;					///< MODEL		:	should be Identity in this example
	glm::mat4	m_view;						///< VIEW		:	this matrix will be modified when moving in the world to look at the object from various point of views
	glm::mat4	m_projection;				///< PROJECTION :	perspective projection matrix created in the constructor and not forecasted to be modified

	CTimer		m_timer;					///< timer to manage the rendering time between 2 frames (and more)

	//C3DNode *	m_root;						///< root node 
	std::vector<C3DObject *>	m_objects;	///< the objects to display
	CCamera*					m_camera;	///< camera to render the objects

	static CApplication * m_instance;		///< instance of the singleton for the current class
};

#endif
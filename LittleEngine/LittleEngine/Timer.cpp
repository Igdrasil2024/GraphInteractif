/// \file	Timer.cpp
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#include "stdafx.h"
#include "Timer.h"


CTimer::CTimer()
{
	start();
}


CTimer::~CTimer()
{
}


void CTimer::start()
{
	m_start = SDL_GetTicks();
	m_previousTick = m_tick = m_start;
	if (m_laps.size())
		m_laps.clear();
}

void CTimer::lap()
{
	Uint32 lap = SDL_GetTicks();
	m_laps.push_back(lap);
}

void CTimer::tickAndLap()
{
	tick();
	m_laps.push_back(m_tick);
}

int CTimer::getNbLaps()
{
	return m_laps.size();
}

float CTimer::getLap(unsigned int p_index)
{
	if (p_index >= m_laps.size())
		return -1.0f;
	return (m_laps[p_index]-m_start)/1000.0f;
}
void CTimer::tick()
{
	m_previousTick = m_tick;
	m_tick = SDL_GetTicks();
}

float CTimer::getDeltaTime()
{
	return (m_tick - m_previousTick) / 1000.0f;
}

float CTimer::getElapsedTime()
{
	return (m_tick - m_start) / 1000.0f;
}

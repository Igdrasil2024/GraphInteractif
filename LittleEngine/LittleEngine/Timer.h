/// \file	Timer.h
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#ifndef _TIMER_
#define _TIMER_

#include "stdafx.h"


/// \class	CTimer
/// \brief	to count the time elapsed
///
/// Enables developer to evaluate time between ticks. Call the tick method to set a new tick. start method reset the time to 0.0
///
class CTimer
{
public:
	CTimer();
	~CTimer();

	/// \brief to set the start time
	void start();
	/// \brief to record the last tick as a lap in the lap list
	void lap();
	/// \brief to generate a new tick and record it in the lap list
	void tickAndLap();
	/// \brief to get the number of laps recorded
	///
	/// \return the number of laps
	///
	int getNbLaps();
	/// \brief to get the lap timestamp in seconds
	///
	/// \param[in]	p_index the number of the lap to get
	///
	/// \return	the timestamp in seconds for the selected lap (-1 if the index is invalid)
	///
	float getLap(unsigned int p_index);
	/// \brief to generate a new tick
	void tick();
	/// \brief to get the elapsed time bewteen the last 2 ticks
	///
	/// \return	the time elapsed in seconds
	///
	float getDeltaTime();
	/// \brief to get the time elapsed since the start call
	///
	/// \return the time elapsed since start call in seconds
	///
	float getElapsedTime();

protected:
	Uint32 m_start;				///< the start time in milliseconds
	Uint32 m_previousTick;			///< the previous tick in milliseconds 
	Uint32 m_tick;					///< the tick in milliseconds
	std::vector<Uint32> m_laps;	//< list of time laps in milliseconds

};

#endif
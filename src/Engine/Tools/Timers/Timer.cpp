/*****************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
*****************************************************************************************/
#include "Timer.h"
#include <stdlib.h>

OE::Tools::Timers::Timer::Timer()
{
#ifdef WIN32
	QueryPerformanceFrequency(&Freq);
	StartCount.QuadPart = 0;
	EndCount.QuadPart = 0;
#else
	StartCount.tv_sec = StartCount.tv_usec = 0;
	EndCount.tv_sec = EndCount.tv_usec = 0;
#endif  
	_dStartTime = 0;
	_dEndTime = 0;
}

OE::Tools::Timers::Timer::~Timer()
{

}

void OE::Tools::Timers::Timer::Start()
{
#ifdef WIN32
	QueryPerformanceCounter(&StartCount);
#else
	gettimeofday(&StartCount, NULL);
#endif
	IsRunning = true;
}

void OE::Tools::Timers::Timer::Stop()
{
#ifdef WIN32
	QueryPerformanceCounter(&EndCount);
#else
	gettimeofday(&EndCount, NULL);
#endif
	IsRunning = false;
}

double OE::Tools::Timers::Timer::GetElapsedMicroSec()
{
#ifdef WIN32
	if(IsRunning)
		QueryPerformanceCounter(&EndCount);

	_dStartTime = StartCount.QuadPart * (1000000.0 / Freq.QuadPart);
	_dEndTime = EndCount.QuadPart * (1000000.0 / Freq.QuadPart);
#else
	if (IsRunning)
		gettimeofday(&EndCount, NULL);

	_dStartTime = (StartCount.tv_sec * 1000000.0) + StartCount.tv_usec;
	_dEndTime = (EndCount.tv_sec * 1000000.0) + EndCount.tv_usec;
#endif
	return _dEndTime - _dStartTime;
}

double OE::Tools::Timers::Timer::GetElapsedMilliSec()
{
	return GetElapsedMicroSec() * 0.001;
}

double OE::Tools::Timers::Timer::GetElapsedSec()
{
	return GetElapsedMilliSec() * 0.001;
}

OE::Tools::Timers::TIME OE::Tools::Timers::Timer::GetElapsed()
{
	TIME temp;
	temp.Microseconds = GetElapsedMicroSec();
	temp.Milliseconds = GetElapsedMilliSec();
	temp.Seconds = GetElapsedSec();
	temp.Minutes = GetElapsedSec() * 0.01666666;
	temp.Hours = temp.Minutes * 0.01666666;
	temp.Days = temp.Hours * 0.04166666;
	temp.Years = temp.Days * 0.00277777;

	return temp;
}

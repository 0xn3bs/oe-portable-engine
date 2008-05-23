/***************************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
***************************************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

namespace Odorless
{
	namespace Engine
	{
		namespace Tools
		{
			namespace Timers
			{
				typedef struct _TIME
				{
					double Years;
					double Days;
					double Hours;
					double Minutes;
					double Seconds;
					double Milliseconds;
					double Microseconds;
				} TIME;

				class Timer
				{
				public:

					Timer();
					~Timer();

					void Start();
					void Stop();

					double GetElapsedMicroSec();
					double GetElapsedMilliSec();
					double GetElapsedSec();
					TIME GetElapsed();
					bool IsRunning;

				protected:
				private:

#ifdef WIN32
					LARGE_INTEGER Freq;
					LARGE_INTEGER StartCount;
					LARGE_INTEGER EndCount;
#else
					timeval StartCount;
					timeval EndCount;
#endif

					double _dStartTime;
					double _dEndTime;

				};
			}
		}
	}
}

#endif /*TIMER_H_*/

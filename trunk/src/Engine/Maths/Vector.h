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
#ifndef COMMON_H_
#define COMMON_H_

#define PI 3.14159265358
#define PI2 1.57079632679

#include <math.h>

//	DO NOT USE ANY OF THESE METHODS YET AS THEY DO NOT WORK AS EXPECTED!

namespace OE
{
	namespace Tools
	{
		namespace Math
		{
			class Common
			{
			public:
				static double osqrt(double num);

				//	This fast piece of code was borrowed from 
				//	http://www.devmaster.net/forums/showthread.php?t=5784
				//	by Nick.

				static float osin(float x)
				{
					const float B = 1.27323954f;
					const float C = -0.405284735f;

					float y = B * x + C * x * x;
#ifdef EXTRA_PRECISION
					//  const float Q = 0.775;
					const float P = 0.225;

					y = P * (y * abs(y) - y) + y;   // Q * y + P * y * abs(y)
#endif
					return y;
				}

				static float ocos(float x)
				{
					return osin(x+1.57079633);
				}

				static float otan(float x)
				{
					return osin(x)/ocos(x);
				}
			};
		}
	}
}

#endif /*COMMON_H_*/

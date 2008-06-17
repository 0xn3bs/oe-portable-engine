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
#ifndef COLOR_H_
#define COLOR_H_

namespace OE
{
	namespace Tools
	{
		namespace Colors
		{
			class Color
			{
			public:
				static void IntToRGB(const unsigned int &RGB, unsigned int *colorComponents);
				static unsigned int *IntToRGB(const unsigned int &RGB);
				static void RGBToInt(int &RGB, const unsigned int &R, const unsigned int &G, const unsigned int &B);
				static unsigned int RGBToInt(const unsigned int &R, const unsigned int &G, const unsigned int &B);

				static void IntToRGBA(const unsigned int &RGBA, unsigned int *colorComponents);
				static unsigned int *IntToRGBA(const unsigned int &RGBA);
				static void RGBAToInt(unsigned int &RGBA, const unsigned int &R,
					const unsigned int &G, const unsigned int &B, const unsigned int &A);
				static unsigned int RGBAToInt(const unsigned int &R, const unsigned int &G, const unsigned int &B,
					const unsigned int &A);
				static void RGBA(unsigned char* RGBA, unsigned char R, unsigned char G, unsigned char B, unsigned char A);

			};
		}
	}
}

#endif /*COLOR_H_*/

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
#ifndef FONT_H_
#define FONT_H_

#include <vector>

namespace OE
{
	namespace UI
	{
		namespace Fonts
		{
			struct _CHARACTER
			{
				float U1, U2, V1, V2;
				float Size;
			};

			struct _FONT
			{
				char Name[255];
				unsigned int TextureHandle;
				std::vector<_CHARACTER> Chars;
			};
		}
	}
}


#endif /*FONT_H_*/

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
namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Fonts
			{
				struct _CHARACTER
				{
					float _fU1, _fU2, _fV1, _fV2;
					float _fSize;
				};
				
				struct _FONT
				{
					char _szName[255];
					unsigned int _uintTextureHandle;
					std::vector<_CHARACTER> _vChars;
				};
			}
		}
	}
}


#endif /*FONT_H_*/

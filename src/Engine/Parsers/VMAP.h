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
#ifndef VMAP_H_
#define VMAP_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace OE
{
	namespace Parsers
	{
		class VMAP
		{
		public:
			struct _SECTION	//	A Multi dimensional tree node.
			{
				int _iStart, _iEnd;
				std::vector<_SECTION*> _vSectionChilds;
			};
			struct _PLANE
			{
				float _3fPos[3][3];
				std::string _sTexture;
			};
			struct _BRUSH
			{
				std::vector<_PLANE> _vPlanes;
			};
			static const int VMAP_LOAD_ERROR = -1;
			static const int VMAP_LOAD_SUCCESS = 0;

			VMAP()
			{
			}
			~VMAP()
			{
			}

			const int ParseVMAP(const char* path);
		private:
			unsigned int _uiLength;
		};
	}
}

#endif

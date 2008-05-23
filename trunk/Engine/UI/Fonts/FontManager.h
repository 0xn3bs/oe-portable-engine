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
#ifndef FONTMANAGER_
#define FONTMANAGER_

#include "Font.h"
#include <vector>
#include <iostream>

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Fonts
			{
				class FontManager
				{
					public:
						FontManager();
						~FontManager();
						const int AddFont(const char *path);
						void SetFont(const char *name);
						void SetFont(const unsigned int &index);
						void CalculateProperUVs(_FONT &font);
						void Write(const char *path) const;
						int IndexOf(const char *name);
						
					private:
						std::vector<Odorless::Engine::UI::Fonts::_FONT> _vFonts;
						int _iCurrentFont;
				};
			}
		}
	}
}

#endif /*FONTMANAGER_*/

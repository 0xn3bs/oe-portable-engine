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
						static const int AddFont(const char *path, bool setMonoSpaced);
						static void SetFont(const char *name);
						static void SetFont(const unsigned int &index);
						static void CalculateUVs(_FONT &font, bool setMonoSpaced);
						static Odorless::Engine::UI::Fonts::_FONT* GetFont(unsigned int &index)
						{
							if(index >= _vFonts.size())
								return (Odorless::Engine::UI::Fonts::_FONT*)0;
						}
						static void Write(const char *path);
						static int IndexOf(const char *name);
						
					private:
						static std::vector<Odorless::Engine::UI::Fonts::_FONT> _vFonts;
						static int _iCurrentFont;
				};
			}
		}
	}
}

#endif /*FONTMANAGER_*/

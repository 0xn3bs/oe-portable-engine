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

#define TEMP_FONT_SIZE 12

#include "Font.h"
#include <vector>
#include <iostream>

namespace OE
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
				static void SetFont(const unsigned int index);
				static void CalculateUVs(_FONT &font, bool setMonoSpaced);
				static OE::UI::Fonts::_FONT* GetFont(unsigned int index)
				{
					if(index >= _vFonts.size())
						return (OE::UI::Fonts::_FONT*)0;
				}
				static void Write(const char *path);
				static void Write(const char c)
				{
					std::string character = std::string(1, c);
					Write(character.c_str());
				}
				static int IndexOf(const char *name);

			private:
				static std::vector<OE::UI::Fonts::_FONT> _vFonts;
				static int _iCurrentFont;
			};
		}
	}
}

#endif /*FONTMANAGER_*/

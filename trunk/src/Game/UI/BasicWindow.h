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
#ifndef BASIC_WINDOW_H_
#define BASIC_WINDOW_H_

#include "Engine/UI/Windows/Window.h"
#include "Engine/Tools/Colors/Color.h"
#include "Engine/Textures/TextureManager.h"
#include "Engine/UI/Widgets/Button.h"

namespace OE
{
	namespace Game
	{
		namespace UI
		{
			class BasicWindow : public OE::UI::Windows::Window
			{
			public:
				BasicWindow() : OE::UI::Windows::Window()
				{
					OE::Tools::Colors::Color::RGBA(_uiBgColor,36,97,104,190);
					OE::Tools::Colors::Color::RGBA(_uiBrdrColor,55,124,129,190);
					OE::Tools::Colors::Color::RGBA(_uiFgColor,236,225,195,190);
					_fTitleBarY = 15.5f;
				}
				BasicWindow(const float x, const float y, const float width, const float height) : OE::UI::Windows::Window(x, y, width, height)
				{
					OE::Tools::Colors::Color::RGBA(_uiBgColor,36,97,104,190);
					OE::Tools::Colors::Color::RGBA(_uiBrdrColor,55,124,129,190);
					OE::Tools::Colors::Color::RGBA(_uiFgColor,236,225,195,190);
					_fTitleBarY = 15.5f;
				}
				~BasicWindow()
				{

				}
				int Initialize()
				{
					return 0;
				}
				void Dispose()
				{
				}
				void Update(const float dt);
				void Render(const float dt);
			};
		}
	}
}

#endif /*BASIC_WINDOW_H_*/

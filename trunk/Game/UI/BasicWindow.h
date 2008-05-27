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

#include "../../Engine/UI/Windows/Window.h"
#include "../../Engine/Tools/Colors/Color.h"
#include "../../Engine/Textures/TextureManager.h"

namespace Odorless
{
	namespace Game
	{
		namespace UI
		{
			class BasicWindow : public Odorless::Engine::UI::Windows::Window
			{
			public:
				BasicWindow() : Odorless::Engine::UI::Windows::Window()
				{
					Engine::Tools::Colors::Color::RGBA(_uiBgColor,36,97,104,190);
					Engine::Tools::Colors::Color::RGBA(_uiBrdrColor,55,124,129,190);
					Engine::Tools::Colors::Color::RGBA(_uiFgColor,255,255,225,190);
					_fTitleBarY = 12.5f / this->_2fDimensions[1];
				}

				BasicWindow(const float &x, const float &y, const float &width, const float &height) : Odorless::Engine::UI::Windows::Window(x, y, width, height)
				{
					Engine::Tools::Colors::Color::RGBA(_uiBgColor,36,97,104,190);
					Engine::Tools::Colors::Color::RGBA(_uiBrdrColor,55,124,129,190);
					Engine::Tools::Colors::Color::RGBA(_uiFgColor,255,255,225,190);
					_fTitleBarY = 12.5f / this->_2fDimensions[1];
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

				void Update(const float &dt)
				{
				}

				void Render(const float &dt)
				{
					//	Background
					glBegin(GL_QUADS);
					glColor4ub(_uiBgColor[0],_uiBgColor[1],_uiBgColor[2],_uiBgColor[3]);
					glTexCoord2f(0, 0);
					glVertex3f(0, 0, 0);
					glTexCoord2f(0, 1);
					glVertex3f(0, 1, 0);
					glTexCoord2f(1, 1);
					glVertex3f(1, 1, 0);
					glTexCoord2f(1, 0);
					glVertex3f(1, 0, 0);
					glColor4ub(_uiBgColor[0]/2,_uiBgColor[1]/2,_uiBgColor[2]/2,_uiBgColor[3]);
					glVertex3f(0, 0, 0);
					glVertex3f(0, _fTitleBarY, 0);
					glVertex3f(1, _fTitleBarY, 0);
					glVertex3f(1, 0, 0);
					glEnd();

					//	Render widgets here.
					for(unsigned int i = 0; i < _vecWidgets.size(); i++)
					{
						_vecWidgets.at(i)->Render(dt);
					}

					//	Border
					glBegin(GL_LINES);
					glColor4ub(_uiBrdrColor[0],_uiBrdrColor[1],_uiBrdrColor[2],_uiBrdrColor[3]);
					glVertex3f(0, 0, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(1, 1, 0);
					glVertex3f(1, 1, 0);
					glVertex3f(1, 0, 0);
					glVertex3f(1, 0, 0);
					glVertex3f(0, 0, 0);
					glVertex3f(0, _fTitleBarY, 0);
					glVertex3f(1, _fTitleBarY, 0);
					glEnd();
				}
			};
		}
	}
}

#endif /*BASIC_WINDOW_H_*/
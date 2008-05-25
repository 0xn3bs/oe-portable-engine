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
					_fTitleBarY = 12.5 / this->_2fDimensions[1];
				}

				BasicWindow(const float &x, const float &y, const float &width, const float &height) : Odorless::Engine::UI::Windows::Window(x, y, width, height)
				{
					_fTitleBarY = 12.5 / this->_2fDimensions[1];
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
					glColor3ub(6, 57, 64);
					glVertex3f(0, 0, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(1, 1, 0);
					glVertex3f(1, 0, 0);
					glColor3ub(62,131,140);
					glVertex3f(0, 0, 0);
					glVertex3f(0, _fTitleBarY, 0);
					glVertex3f(1, _fTitleBarY, 0);
					glVertex3f(1, 0, 0);
					glEnd();

					//	Render widgets here.
					for(int i = 0; i < _vecWidgets.size(); i++)
					{
						_vecWidgets.at(i)->Render(dt);
					}

					//	Border
					glBegin(GL_LINES);
					glColor3ub(25,94,99);
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

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

				}

				BasicWindow(const float &x, const float &y, const float &width, const float &height) : Odorless::Engine::UI::Windows::Window(x, y, width, height)
				{

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
					glBegin(GL_QUADS);
					glColor3f(1, 0, 0);
					//Top-left vertex (corner)
					//glTexCoord2f(character._fU1, character._fV1);
					glVertex3f(0, 0, 0);

					//Bottom-left vertex (corner)
					//glTexCoord2f(character._fU1, character._fV2);
					glVertex3f(0, 1, 0);

					//Bottom-right vertex (corner)
					//glTexCoord2f(character._fU2, character._fV2);
					glVertex3f(1, 1, 0);

					//Top-right vertex (corner)
					//glTexCoord2f(character._fU2, character._fV1);
					glVertex3f(1, 0, 0);
					glEnd();
				}
			};
		}
	}
}

#endif /*BASIC_WINDOW_H_*/

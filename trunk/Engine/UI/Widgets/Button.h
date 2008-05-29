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
#ifndef BUTTON_H_
#define BUTTON_H_

#include "Engine/UI/Fonts/FontManager.h"

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Widgets
			{
				class Button : public Odorless::Engine::UI::Widgets::Widget
				{
					public:
						Button(const float& x, const float &y, const float &width, const float &height, 
							Odorless::Engine::UI::Windows::Window* parentWindow) : Widget(x, y, width, 
							height, parentWindow)
						{

						}
						~Button()
						{
						}
						virtual void Update(const float &dt)
						{

						}
						virtual void Render(const float &dt)
						{
							
							glPushMatrix();
							glBegin(GL_QUADS);
							glVertex3f(0, 0, 0);
							glVertex3f(0, _2fDimensions[1], 0);
							glTexCoord2f(1, 1);
							glVertex3f(_2fDimensions[0], _2fDimensions[1], 0);
							glTexCoord2f(1, 0);
							glVertex3f(_2fDimensions[0], 0, 0);
							glEnd();
							glPopMatrix();
							glColor4ub(150,240,150,255);
							Engine::UI::Fonts::FontManager::Write(_szCaption);
						}
				};
			}
		}
	}
}

#endif /*BUTTON_H_*/

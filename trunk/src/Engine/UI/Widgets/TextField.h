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
#ifndef TEXTFIELD_H_
#define TEXTFIELD_H_

#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/UI/Widgets/Widget.h"

namespace OE
{
	namespace UI
	{
		namespace Widgets
		{
			class TextField : public OE::UI::Widgets::Widget
			{
			public:
				TextField(const float x, const float y, const float width, const float height, 
					OE::UI::Windows::Window* parentWindow) : Widget(x, y, width, 
					height, parentWindow)
				{
					_uiBgColor[0] = 102;
					_uiBgColor[1] = 102;
					_uiBgColor[2] = 51;
					_szCaption = "";
				}

				~TextField()
				{
				}

				virtual void OnMouseButton(const int button, const int action)
				{
				}

				virtual void OnMouseOver()
				{
					_uiBgColor[0] += 100;
					_uiBgColor[2] += 100;
				}

				virtual void OnMouseOut()
				{
					_uiBgColor[0] -= 100;
					_uiBgColor[2] -= 100;
				}

				virtual void OnMouseClick()
				{

				}

				virtual void OnKeyEvent(const int key, const int action)
				{
					if(_bHasFocus)
					{
						if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
							_szCaption = "";

						if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
						{
							if(_szCaption.length()>0)
								_szCaption.erase(_szCaption.end()-1);
						}
					}
				}

				virtual void OnCharEvent(const int key, const int action)
				{
					if(_bHasFocus && action == GLFW_PRESS)
					{
						_szCaption += key;
					}
				}

				virtual void Update(const float dt)
				{
					if(_bHasFocus)
					{
						std::cout << "Textfield has focus" << std::endl;
						_uiBgColor[0] = 100;
						_uiBgColor[2] = 100;
					}
					else
					{
						std::cout << "Textfield doesn't have focus" << std::endl;
						_uiBgColor[0] = 50;
						_uiBgColor[2] = 50;
					}
				}

				virtual void Render(const float dt)
				{
					glColor4ub(_uiBgColor[0],_uiBgColor[1],_uiBgColor[2],_uiBgColor[3]);
					glBegin(GL_QUADS);
					glVertex3f(0, 0, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(1, 1, 0);
					glVertex3f(1, 0, 0);
					glEnd();
					glColor4ub(255,255,255,255);
					glScalef(TEMP_FONT_SIZE/_v2fDimensions.x, TEMP_FONT_SIZE/_v2fDimensions.y, 1);
					OE::UI::Fonts::FontManager::Write(_szCaption.c_str());
					glScalef(1/(TEMP_FONT_SIZE/_v2fDimensions.x), 1/(TEMP_FONT_SIZE/_v2fDimensions.y), 1);
				}
			};
		}
	}
}

#endif /*TEXTFIELD_H_*/

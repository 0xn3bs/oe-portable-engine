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
			private:
				std::string _szTextWithCaret;
				int _caretPos;
				void (*OnSubmit)(const char* text);
			public:
				TextField(const float x, const float y, const float width, const float height, 
					OE::UI::Windows::Window* parentWindow) : Widget(x, y, width, 
					height, parentWindow)
				{
					_uiBgColor[0] = 102;
					_uiBgColor[1] = 102;
					_uiBgColor[2] = 51;
					_szCaption = "";
					_szTextWithCaret = "";
					_caretPos = 0;
				}

				~TextField()
				{
				}

				void SetSubmitCallback(void (*cb)(const char* text))
				{
					OnSubmit = cb;
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
					int mX, mY, lX, lY;
					lX = lY = mX = mY = 0;
					OE::Input::InputManager::GetMousePos(&mX, &mY);
					GetLocalPosition(&lX, &lY);

					_caretPos = (_szCaption.length() - (_szCaption.length() - lX))/16;

					_szTextWithCaret = _szCaption;

					if(mX > (_szCaption.length()+1) * 16)
					{
						if(_szCaption.length()==0)
						{
							_caretPos = 0;
							_szTextWithCaret.push_back('|');
						}
						else
						{
							_caretPos = _szCaption.length();
							_szTextWithCaret.push_back('|');
						}
					}
					else
						_szTextWithCaret.insert(_szTextWithCaret.begin()+_caretPos, '|');
				}

				virtual void OnKeyEvent(const int key, const int action)
				{
					if(_bHasFocus)
					{
						if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
						{
							if(_caretPos > 0)
								_caretPos--;
						}
						if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
						{
							if(_caretPos < _szCaption.length())
								_caretPos++;
						}
						if(key == GLFW_KEY_ENTER && action == GLFW_PRESS)
						{
							if(*OnSubmit != NULL)
							{
								OnSubmit(_szCaption.c_str());
							}
							_szTextWithCaret = "";
							_szCaption = "";
							_caretPos = 0;
						}
						if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS)
						{
							if(_szCaption.length()>0)
							{
								_szCaption.erase(_szCaption.begin() + _caretPos-1,_szCaption.begin() + _caretPos);
								_caretPos--;
							}
						}
						_szTextWithCaret = _szCaption;
						_szTextWithCaret.insert(_szTextWithCaret.begin()+_caretPos, '|');
					}
				}

				virtual void OnCharEvent(const int key, const int action)
				{
					if(_bHasFocus && action == GLFW_PRESS)
					{
						_szCaption.insert(_szCaption.begin()+_caretPos, (char)key);
						_szTextWithCaret = _szCaption;
						_caretPos++;
						_szTextWithCaret.insert(_szTextWithCaret.begin()+_caretPos, '|');
					}
				}

				virtual void Update(const float dt)
				{
					if(_bHasFocus)
					{
						_uiBgColor[0] = 100;
						_uiBgColor[2] = 100;
					}
					else
					{
						_szTextWithCaret = _szCaption;
						_caretPos = _szCaption.length();
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
					glScalef(1.0f/_v2fDimensions.x, 1.0f/_v2fDimensions.y, 1);
					OE::UI::Fonts::FontManager::Write(_szTextWithCaret.c_str());
					glScalef(1/(1.0f/_v2fDimensions.x), 1/(1.0f/_v2fDimensions.y), 1);
				}
			};
		}
	}
}

#endif /*TEXTFIELD_H_*/

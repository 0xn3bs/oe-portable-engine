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
				int _caretPos;
				float _iTotalTime;
				bool _bCaretRender;
				void (*OnSubmit)(const char* text);
			public:
				TextField(const float x, const float y, const float width, const float height, 
					OE::UI::Windows::Window* parentWindow) : Widget(x, y, width, 
					height, parentWindow)
				{
					_uiBgColor[0] = 50;
					_uiBgColor[1] = 102;
					_uiBgColor[2] = 100;
					_uiBgColor[3] = 150;
					_szCaption = "";
					_caretPos = 0;
					_bCaretRender = false;
					_iTotalTime = 0;
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
					if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
					{
						int mX, mY, lX, lY;
						lX = lY = mX = mY = 0;
						OE::Input::InputManager::GetMousePos(&mX, &mY);
						GetLocalPosition(&lX, &lY);

						std::cout << "X: " << lX << " Y: " << lY << std::endl;

						_caretPos = lX/16;

						if(lX > (_szCaption.length()+1) * 16)
						{
							if(_szCaption.length()==0)
							{
								_caretPos = 0;
							}
							else
							{
								_caretPos = _szCaption.length();
							}
						}
					}
				}

				virtual void OnMouseOver()
				{
				}

				virtual void OnMouseOut()
				{
				}

				virtual void OnMouseClick()
				{
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
						if(key == GLFW_KEY_DEL && action == GLFW_PRESS)
						{
							if(_caretPos < _szCaption.length())
							{
								_szCaption.erase(_szCaption.begin() + _caretPos,_szCaption.begin() + _caretPos+1);
							}
						}
					}
				}

				virtual void OnCharEvent(const int key, const int action)
				{
					if(_bHasFocus && action == GLFW_PRESS)
					{
						_szCaption.insert(_szCaption.begin()+_caretPos, (char)key);
						_caretPos++;
					}
				}

				virtual void Update(const float dt)
				{
					if(_iTotalTime < 0.5f)
					{
						_iTotalTime+=dt;
					}
					else
					{
						_bCaretRender = !_bCaretRender;
						_iTotalTime = 0;
					}

					if(!_bHasFocus)
					{
						_caretPos = 0;
						_uiBgColor[0] = 100;
						_uiBgColor[2] = 100;
					}
					else
					{
						_uiBgColor[0] = 50;
						_uiBgColor[1] = 102;
						_uiBgColor[2] = 100;
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
					

					glScalef(1.0f/_v2fDimensions.x, 1.0f/_v2fDimensions.y, 1);
					
					glColor4ub(255,255,255,255);
					OE::UI::Fonts::FontManager::Write(_szCaption.c_str());
					glColor4ub(153,255,255,255);
					if(_bHasFocus && _bCaretRender)
					{
						float xTrans = (_v2fPosition.x + (_caretPos * 16))-8;
						glTranslatef(xTrans, 0, 0);
						OE::UI::Fonts::FontManager::Write((char)127);
						glTranslatef(-xTrans, 0, 0);
					}

					glScalef(1/(1.0f/_v2fDimensions.x), 1/(1.0f/_v2fDimensions.y), 1);
				}
			};
		}
	}
}

#endif /*TEXTFIELD_H_*/

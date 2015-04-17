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
#include "Engine/Maths/Math.h"
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

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
				int _caretUpPos;
				float _iTotalTime;
				bool _bCaretRender;
				bool _bControlPressed;
				std::string _szRenderedText;
				int _iLeftBound, _iRightBound;
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
					_caretUpPos = 0;
					_bCaretRender = false;
					_iTotalTime = 0;
					_bControlPressed = false;
					_iLeftBound  = 0;
					_iRightBound = 0;
				}

				~TextField()
				{
				}

				void SetSubmitCallback(void (*cb)(const char* text))
				{
					OnSubmit = cb;
				}

				void CalculateCaretPosition(int *caretPos)
				{
					double mX, mY;
					int lX, lY;
					lX = lY = 0;
					mX = mY = 0;
					OE::Input::InputManager::GetMousePos(&mX, &mY);
					GetLocalPosition(&lX, &lY);

					*caretPos = lX/16;

					if(lX > (_szCaption.length()) * 16)
					{
						if(_szCaption.length()==0)
						{
							*caretPos = 0;
						}
						else
						{
							*caretPos = _szCaption.length();
						}
					}
				}

				virtual void OnMouseButton(const int button, const int action)
				{
					if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && _bHasFocus)
					{
						CalculateCaretPosition(&_caretPos);
						_caretUpPos = _caretPos;
					}

					if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && _bHasFocus)
					{
						CalculateCaretPosition(&_caretUpPos);
					}
				}

				virtual void OnMouseMove(const int x, const int y)
				{
					if (OE::Input::InputManager::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && _bHasFocus)
					{
						CalculateCaretPosition(&_caretUpPos);
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

				virtual void OnKeyEvent(const int key, const int action, const int mods);

				virtual void OnCharEvent(const int codepoint);

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
					float carStartX = OE::Maths::Math::SafeInverse(_winParentWindow->GetXPos());
					carStartX += OE::Maths::Math::SafeInverse(_v2fPosition.x) + (_caretPos * 16);
					float carEndX = OE::Maths::Math::SafeInverse(_winParentWindow->GetXPos());
					carEndX += OE::Maths::Math::SafeInverse(_v2fPosition.x)+ (_caretUpPos * 16);

					glColor4ub(_uiBgColor[0],_uiBgColor[1],_uiBgColor[2],_uiBgColor[3]);
					glBegin(GL_QUADS);
					glVertex3f(0, 0, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(1, 1, 0);
					glVertex3f(1, 0, 0);
					glEnd();

					glColor4ub(0,0,0,255);
					glBegin(GL_QUADS);
					glVertex3f(carStartX/_v2fDimensions.x, 0, 0);
					glVertex3f(carStartX/_v2fDimensions.x, 1, 0);
					glVertex3f(carEndX/_v2fDimensions.x, 1, 0);
					glVertex3f(carEndX/_v2fDimensions.x, 0, 0);
					glEnd();

					glScalef(OE::Maths::Math::SafeInverse(_v2fDimensions.x), OE::Maths::Math::SafeInverse(_v2fDimensions.y), 1);

					glColor4ub(255,255,255,255);
					OE::UI::Fonts::FontManager::Write(_szRenderedText.c_str());
					glColor4ub(153,255,255,255);
					if(_bHasFocus && _bCaretRender)
					{
						float xTrans = OE::Maths::Math::SafeInverse(_winParentWindow->GetXPos());
						xTrans += ((OE::Maths::Math::SafeInverse(_v2fPosition.x) + _caretPos * 16) - 8);
						glTranslatef(xTrans, 0, 0);
						OE::UI::Fonts::FontManager::Write('_');
						glTranslatef(-xTrans, 0, 0);
					}

					glBegin(GL_LINES);
					glVertex3f(carStartX, 0, 0);
					glVertex3f(carEndX, 0, 0);
					glEnd();

					glScalef(OE::Maths::Math::SafeInverse(OE::Maths::Math::SafeInverse(_v2fDimensions.x)), OE::Maths::Math::SafeInverse(OE::Maths::Math::SafeInverse(_v2fDimensions.y)), 1);
				}
			};
		}
	}
}

#endif /*TEXTFIELD_H_*/

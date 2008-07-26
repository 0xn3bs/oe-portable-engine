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
#ifndef LISTBOX_H_
#define LISTBOX_H_

#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/UI/Widgets/Widget.h"
#include <string>
#include <vector>

namespace OE
{
	namespace UI
	{
		namespace Widgets
		{
			class ListBox : public OE::UI::Widgets::Widget
			{
			private:
				std::vector<std::string> _vItems;
				void (*OnItemSelected)(int index, const char* value);
			public:
				ListBox(const float x, const float y, const float width, const float height, 
					OE::UI::Windows::Window* parentWindow) : Widget(x, y, width, 
					height, parentWindow)
				{
					_uiBgColor[0] = 102;
					_uiBgColor[1] = 102;
					_uiBgColor[2] = 51;
					_szCaption = "";
				}

				~ListBox()
				{
				}

				virtual void PushBack(const char* value)
				{
					_vItems.push_back(std::string(value));
				}

				virtual void OnMouseButton(const int button, const int action)
				{
				}

				virtual void OnMouseOver()
				{
				}

				virtual void OnMouseOut()
				{
				}

				virtual void OnMouseClick()
				{
					int mX, mY, lX, lY;
					lX = lY = mX = mY = 0;
					OE::Input::InputManager::GetMousePos(&mX, &mY);
					GetLocalPosition(&lX, &lY);;
				}

				virtual void OnKeyEvent(const int key, const int action)
				{
				}

				virtual void OnCharEvent(const int key, const int action)
				{
				}

				virtual void Update(const float dt)
				{
				}

				virtual void Render(const float dt)
				{
					glColor4ub(_uiBgColor[3],_uiBgColor[1],_uiBgColor[2],255);

					glScalef(OE::Maths::Math::SafeInverse(_v2fDimensions.x), OE::Maths::Math::SafeInverse(_v2fDimensions.y), 1);
					for(int i = 0; i < _vItems.size(); i++)
					{
						if((i+1)*16 >= _v2fDimensions.y)
							break;

						glTranslatef(0, (i*16), 0);
						OE::UI::Fonts::FontManager::Write(_vItems[i].c_str());
						glTranslatef(0, -(i*16), 0);
					}
					glScalef(OE::Maths::Math::SafeInverse(OE::Maths::Math::SafeInverse(_v2fDimensions.x)), OE::Maths::Math::SafeInverse(OE::Maths::Math::SafeInverse(_v2fDimensions.y)), 1);
				}
			};
		}
	}
}

#endif /*LIST_BOX_*/

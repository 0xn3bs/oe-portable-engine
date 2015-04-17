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
#ifndef WIDGET_H_
#define WIDGET_H_

#include "Engine/UI/Windows/Window.h"
#include "Engine/Maths/Vector.h"
#include "Engine/Input/Input.h"
#include <string>

namespace OE
{
	namespace UI
	{
		namespace Windows{class Window;}	//	Forward declaration for Window.
		namespace Widgets
		{
			class Widget
			{
			protected:
				void GetLocalPosition(int *x, int *y);
			public:
				Widget(const float x, const float y, const float width, const float height, 
					OE::UI::Windows::Window* parentWindow)
				{
					_v2fPosition.x = x;
					_v2fPosition.y = y;
					_v2fDimensions.x = width;
					_v2fDimensions.y = height;
					_winParentWindow = parentWindow;
					_uiBgColor[0] = 150;
					_uiBgColor[1] = 240;
					_uiBgColor[2] = 150;
					_uiBgColor[3] = 255;
					_bIsOver = false;
					_bHasFocus = false;
				}

				~Widget()
				{
				}

				virtual void OnMouseOver(){};
				virtual void OnMouseOut(){};
				virtual void OnMouseClick(){};
				virtual void OnMouseMove(const int x, const int y){};
				virtual void OnMouseButton(const int button, const int action){};
				virtual void OnCharEvent(const int key, const int action){};
				virtual void OnKeyEvent(const int key, const int action, const int mods){};

				virtual void Update(const float dt) = 0;
				virtual void Render(const float dt) = 0;
				bool IsOver(const float x, const float y);
				void SetCaption(const char* caption)
				{
					_szCaption = std::string(caption);
				}

				OE::UI::Windows::Window* _winParentWindow;
				std::string _szCaption;

				OE::Maths::Vec2<float> _v2fDimensions;
				OE::Maths::Vec2<float> _v2fPosition;

				unsigned char _uiFgColor[4];
				unsigned char _uiBgColor[4];
				unsigned char _uiBrdrColor[4];
				bool _bIsOver;
				bool _bHasFocus;
			};
		}
	}
}

#endif /*WIDGET_H_*/

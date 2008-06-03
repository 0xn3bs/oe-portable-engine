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

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Windows{class Window;}	//	Forward declaration for Window.
			namespace Widgets
			{
				class Widget
				{
					public:
						Widget(const float &x, const float &y, const float &width, const float &height, 
							Odorless::Engine::UI::Windows::Window* parentWindow)
						{
							_2fPosition[0] = x;
							_2fPosition[1] = y;
							_2fDimensions[0] = width;
							_2fDimensions[1] = height;
							_winParentWindow = parentWindow;
							_uiBgColor[0] = 150;
							_uiBgColor[1] = 240;
							_uiBgColor[2] = 150;
							_uiBgColor[3] = 255;
							_bIsOver = false;
						}
						~Widget()
						{
						}
						virtual void OnMouseOver(){};
						virtual void OnMouseOut(){};
						virtual void OnMouseClick(){};
						virtual void OnMouseButton(const int button, const int action){};
						virtual void Update(const float &dt) = 0;
						virtual void Render(const float &dt) = 0;
						bool IsOver(const float &x, const float &y);
						void SetCaption(char* caption)
						{
							_szCaption = caption;
						}
						Odorless::Engine::UI::Windows::Window* _winParentWindow;
						char* _szCaption;
						float _2fDimensions[2];
						float _2fPosition[2];
						unsigned char _uiFgColor[4];
						unsigned char _uiBgColor[4];
						unsigned char _uiBrdrColor[4];
						bool _bIsOver;
				};
			}
		}
	}
}

#endif /*WIDGET_H_*/

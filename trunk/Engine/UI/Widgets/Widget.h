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
							_winParentWindow = parentWindow;
						}
						~Widget();
						virtual void OnMouseClick(const int x, const int y){};
						virtual void OnMouseButton(const int button, const int action){};
						virtual void Update(const float &dt) = 0;
						virtual void Render(const float &dt) = 0;
						
					private:
						Odorless::Engine::UI::Windows::Window* _winParentWindow;
						float _2fDimension[2];
						float _2fPosition[2];
						unsigned char _uiFgColor[4];
						unsigned char _uiBgColor[4];
						unsigned char _uiBrdrColor[4];
				};
			}
		}
	}
}

#endif /*WIDGET_H_*/

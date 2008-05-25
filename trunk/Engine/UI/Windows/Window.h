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
#ifndef WINDOW_H_
#define WINDOW_H_

#include <GL/glfw.h>
#include "../Widgets/Widget.h"

#include <vector>

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Windows
			{
				class WindowManager; //	Forward declaration for the Window Manager.
				class Window
				{
					friend class WindowManager;
					public:
						Window();
						Window(const float &x, const float &y, const float &width, const float &height);
						~Window();
						virtual int Initialize() = 0;
						virtual void Dispose() = 0;
						virtual void Update(const float &dt) = 0;
						virtual void Render(const float &dt) = 0;
						bool IsOver(const float &x, const float &y);
						bool IsOverTitleBar(const float &x, const float &y);
						void AddWidget(const UI::Widgets::Widget &widget);
						void RemoveWidget(const unsigned int &index);
					
					protected:
						float _2fDimensions[2];
						float _2fPosition[2];
						float _fTitleBarY;
						unsigned int _uiFgColor;
						unsigned int _uiBgColor;
						std::vector<UI::Widgets::Widget*> _vecWidgets;
				};
			}
		}
	}
}

#endif /*WINDOW_H_*/

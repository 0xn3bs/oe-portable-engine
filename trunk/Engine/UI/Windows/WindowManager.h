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
#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include "../../../Game/UI/BasicWindow.h"
#include "Window.h"

#include <vector>

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Windows
			{
				class WindowManager
				{
					public:
						WindowManager()
						{
						}
						~WindowManager()
						{
						}
						void AddWindow(Window &window)
						{
							_vecWindows.push_back(&window);
						}
						void RemoveWindow(const unsigned int &index)
						{
							_vecWindows.erase(_vecWindows.begin() + index);
						}
						void Update(const float &dt)
						{
							for(int i = 0; i < _vecWindows.size(); i++)
							{
								_vecWindows.at(i)->Update(dt);
							}
						}
						void UpdateWin(const int width, const int height)
						{
							_iWinWidth = width;
							_iWinHeight = height;
						}
						void Render(const float &dt)
						{
							for(int i = 0; i < _vecWindows.size(); i++)
							{
								_vecWindows.at(i)->Render(dt, _iWinWidth, _iWinHeight);
							}
						}
					
					private:
						std::vector<Window*> _vecWindows;
						int _iWinWidth, _iWinHeight;
				};
			}
		}
	}
}

#endif /*WINDOWMANAGER_H_*/

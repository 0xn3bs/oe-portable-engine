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
							_iWinWidth = 0;
							_iWinHeight = 0;
						}
						~WindowManager()
						{
						}
						void SetInput(Odorless::Engine::Input::InputManager* inputManager)
						{
							_inputManager = inputManager;
						}
						void AddWindow(Window &window)
						{
							_vecWindows.push_back(&window);
						}
						void RemoveWindow(const unsigned int &index)
						{
							_vecWindows.erase(_vecWindows.begin() + index);
						}
						void SetFocus(const unsigned int &index)
						{
							Window *tempWinA = _vecWindows.at(index);
							_vecWindows.erase(_vecWindows.begin() + index);
							_vecWindows.push_back(tempWinA);
						}
						void Update(const float &dt);
						void UpdateWin(const int width, const int height);
						void Render(const float &dt);
					
					private:
						Odorless::Engine::Input::InputManager* _inputManager;
						//Odorless::Engine::UI::Fonts::FontManager fontManager;
						std::vector<Window*> _vecWindows;
						int _iWinWidth, _iWinHeight;
				};
			}
		}
	}
}

#endif /*WINDOWMANAGER_H_*/

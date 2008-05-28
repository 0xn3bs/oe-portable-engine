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

#include "Window.h"
#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/Input/Input.h"

#include <vector>
#include <iostream>

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Windows
			{
				class WindowManager : public Odorless::Engine::Input::InputListener
				{
				public:
					WindowManager()
					{
						_iWinWidth = 0;
						_iWinHeight = 0;
						Odorless::Engine::Input::InputManager::AddInputListener(this);
					}

					~WindowManager()
					{
					}

					void OnMouseDown(const int x, const int y)
					{
						std::cout << "OMG MOUSE DOWN" << std::endl;
					}

					void AddWindow(Window &window)
					{
						_vecWindows.push_back(&window);
						SetFocus(_vecWindows.size()-1);
					}
					void RemoveWindow(const unsigned int &index)
					{
						_vecWindows.at(index)->_bHasFocus = false;
						_vecWindows.erase(_vecWindows.begin() + index);
					}
					bool CanPick(const unsigned int &index, const unsigned int &x, const unsigned int &y)
					{
						bool bOver = _vecWindows.at(index)->IsOver(x, y);
						for(int i = index+1; i < _vecWindows.size(); i++)
						{
							bool b = (_vecWindows.at(i)->IsOver(x,y) || _vecWindows.at(i)->IsOverTitleBar(x,y));
							if(b)
								return false;
						}

						return bOver;
					}
					void SetFocus(const unsigned int &index)
					{
						if(_winCurrentFocused != NULL) 
							_winCurrentFocused->_bHasFocus = false;
						_winCurrentFocused = _vecWindows.at(index);
						_winCurrentFocused->_bHasFocus = true;
						_vecWindows.erase(_vecWindows.begin()+index);
						_vecWindows.push_back(_winCurrentFocused);
					}
					void Update(const float &dt);
					void UpdateWin(const int width, const int height);
					void Render(const float &dt);

				private:
					bool _bIsMouseAlreadyDown;
					Window *_winCurrentFocused;
					std::vector<Window*> _vecWindows;

					int _iWinWidth, _iWinHeight;
				};
			}
		}
	}
}

#endif /*WINDOWMANAGER_H_*/

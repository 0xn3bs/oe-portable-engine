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
					void OnMouseClick(const char button, const int x, const int y)
					{
					}
					void OnMouseDown(const char button, const int x, const int y)
					{
					}
					void OnMouseUp(const char button, const int x, const int y)
					{
					}
					void SetInput(Odorless::Engine::Input::InputManager* inputManager)
					{
						_inputManager = inputManager;
						//_inputManager->AddMouseDownCB(OnMouseDown);
						//_inputManager->AddMouseUpCB(this, OnMouseUp);
						//_inputManager->AddMouseClickCB(this, OnMouseClick);
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

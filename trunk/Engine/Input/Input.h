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

#ifndef INPUT_H_
#define INPUT_H_

#include <GL/glfw.h>
#include <ostream>
#include <memory>
#include <vector>

namespace Odorless
{
	namespace Engine
	{
		namespace Input
		{
			class InputManager
			{
			private:
				//template <typename T> std::vector<void(T::*)(const char button, const int x, const int y)> _vecMouseDownCallbacks;
				//template <typename T> std::vector<void(T::*)(const char button, const int x, const int y)> _vecMouseUpCallbacks;
				//template <typename T> std::vector<void(T::*)(const char button, const int x, const int y)> _vecMouseClickCallbacks;

				int _iMouseClckStrtX;
				int _iMouseClckStrtY;
				int _iMouseX;
				int _iMouseY;
				int _iMouseDeltaX;
				int _iMouseDeltaY;
				static bool _rgcKeys[255];

				static void GLFWCALL SetKeyEvent(const int key, const int action)
				{
			
					_rgcKeys[key] = (bool)action;
				}
			public:
				InputManager()
				{
					_iMouseClckStrtX=0;
					_iMouseClckStrtY=0;
					_iMouseX=0;
					_iMouseY=0;
					_iMouseDeltaX=0;
					_iMouseDeltaY=0;
				}

				~InputManager()
				{
				}

				void Initialize()
				{
					glfwSetKeyCallback(SetKeyEvent);
				}

				void Update()
				{
					int iMouseOldX = _iMouseX;
					int iMouseOldY = _iMouseY;
					glfwGetMousePos(&_iMouseX, &_iMouseY);

					_iMouseDeltaX = _iMouseX - iMouseOldX;
					_iMouseDeltaY = _iMouseY - iMouseOldY;
				}

				/*
				void AddMouseDownCB(void(*mDownCB)(const char button, const int x, const int y))
				{
					_vecMouseDownCallbacks.push_back(mDownCB);
				}

				void AddMouseUpCB(void(*mDownCB)(const char button, const int x, const int y))
				{
					_vecMouseUpCallbacks.push_back(mDownCB);
				}

				void AddMouseClickCB(void(*mClickCB)(const char button, const int x, const int y))
				{
					_vecMouseClickCallbacks.push_back(mClickCB);
				}
				*/

				void SetKeyState(const char key, bool state)
				{
					_rgcKeys[key] = state;
				}

				static bool GetKeyState(const char key)
				{
					return _rgcKeys[key];
				}

				void SetMousePos(const int x, const int y)
				{
					glfwSetMousePos(x, y);
				}

				void SetMouseX(const int x)
				{
					glfwSetMousePos(x, _iMouseY);
				}

				void SetMouseY(const int y)
				{
					glfwSetMousePos(_iMouseX, y);
				}

				bool IsMouseDown(const int &button)
				{
					return glfwGetMouseButton(button);
				}

				void GetMousePos(int *xPos, int *yPos)
				{
					*xPos = _iMouseX;
					*yPos = _iMouseY;
				}

				const int GetMouseDeltaX()
				{
					return _iMouseDeltaX;
				}

				const int GetMouseDeltaY()
				{
					return _iMouseDeltaY;
				}

				const int GetMouseX()
				{
					return _iMouseX;
				}

				const int GetMouseY()
				{
					return _iMouseY;
				}
			};
		};
	};
};
#endif /*INPUT_H_*/

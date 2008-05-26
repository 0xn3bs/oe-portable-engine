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

namespace Odorless
{
	namespace Engine
	{
		namespace Input
		{
			class InputManager
			{
			private:
				static int _iMouseX;
				static int _iMouseY;
				static int _iMouseDeltaX;
				static int _iMouseDeltaY;
				static bool _rgcKeys[255];

				static void GLFWCALL SetKeyEvent(const int key, const int action)
				{
					_rgcKeys[key] = (bool)action;
				}
			public:
				InputManager()
				{
				}

				~InputManager()
				{
				}

				static void Initialize()
				{
					glfwSetKeyCallback(SetKeyEvent);
				}

				static void Update()
				{
					int iMouseOldX = _iMouseX;
					int iMouseOldY = _iMouseY;
					glfwGetMousePos(&_iMouseX, &_iMouseY);

					_iMouseDeltaX = _iMouseX - iMouseOldX;
					_iMouseDeltaY = _iMouseY - iMouseOldY;
				}

				static void SetKeyState(const char key, bool state)
				{
					_rgcKeys[key] = state;
				}

				static bool GetKeyState(const char key)
				{
					return _rgcKeys[key];
				}

				static void SetMousePos(const int x, const int y)
				{
					glfwSetMousePos(x, y);
				}

				static void SetMouseX(const int x)
				{
					glfwSetMousePos(x, _iMouseY);
				}

				static void SetMouseY(const int y)
				{
					glfwSetMousePos(_iMouseX, y);
				}

				static bool IsMouseDown(const int &button)
				{
					return glfwGetMouseButton(button);
				}

				static void GetMousePos(int *xPos, int *yPos)
				{
					*xPos = _iMouseX;
					*yPos = _iMouseY;
				}

				static const int GetMouseDeltaX()
				{
					return _iMouseDeltaX;
				}

				static const int GetMouseDeltaY()
				{
					return _iMouseDeltaY;
				}

				static const int GetMouseX()
				{
					return _iMouseX;
				}

				static const int GetMouseY()
				{
					return _iMouseY;
				}
			};
		};
	};
};
#endif /*INPUT_H_*/

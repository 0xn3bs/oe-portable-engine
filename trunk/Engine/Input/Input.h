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
#include <iostream>
#include <memory>
#include <vector>

namespace Odorless
{
	namespace Engine
	{
		namespace Input
		{
			class InputListener
			{
			public:
				virtual void OnMousePress(const int x, const int y){};
				virtual void OnMouseDown(const int x, const int y){};
				virtual void OnMouseUp(const int x, const int y){};
				virtual void OnMouseClick(const int x, const int y){};
			};

			class InputManager
			{
			private:
				static std::vector<InputListener*> _vecInputListeners;

				static int _iMouseX;
				static int _iMouseY;
				static int _iMouseDeltaX;
				static int _iMouseDeltaY;
				static int _iMousePressStartX;
				static int _iMousePressStartY;
				static bool _rgcKeys[255];

				static void GLFWCALL GLFWSetKeyEvent(const int key, const int action)
				{
					_rgcKeys[key] = (bool)action;
				}

				static void GLFWCALL GLFWSetMousePos(const int x, const int y)
				{
					_iMouseDeltaX = x - _iMouseX;
					_iMouseDeltaY = y - _iMouseY;
					_iMouseX = x;
					_iMouseY = y;
				}

				static void GLFWCALL GLFWSetMouseDown(const int x, const int y)
				{
					for(int i = 0; i < _vecInputListeners.size(); i++)
					{
						_vecInputListeners.at(i)->OnMouseDown(x, y);
					}
				}
			public:
				InputManager()
				{
					_iMousePressStartX=0;
					_iMousePressStartY=0;
					_iMouseX=0;
					_iMouseY=0;
					_iMouseDeltaX=0;
					_iMouseDeltaY=0;
					glfwSetKeyCallback(GLFWSetKeyEvent);
					glfwSetMousePosCallback(GLFWSetMousePos);
					glfwSetMouseButtonCallback(GLFWSetMouseDown);
				}

				~InputManager()
				{
				}

				static void Initialize()
				{
					
				}

				static void Update()
				{
					int x, y;
					glfwGetMousePos(&x, &y);
					_iMouseDeltaX = x - _iMouseX;
					_iMouseDeltaY = y - _iMouseY;
					_iMouseX = x;
					_iMouseY = y;
				}

				static void AddInputListener(InputListener* inputListener)
				{
					_vecInputListeners.push_back(inputListener);
				}
				
				static void SetKeyState(const char &key, bool &state)
				{
					_rgcKeys[key] = state;
				}

				static bool GetKeyState(const char &key)
				{
					return _rgcKeys[key];
				}

				static void SetMousePos(const int &x, const int &y)
				{
					glfwSetMousePos(x, y);
				}

				static void SetMouseX(const int &x)
				{
					glfwSetMousePos(x, _iMouseY);
				}

				static void SetMouseY(const int &y)
				{
					glfwSetMousePos(_iMouseX, y);
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
				
				static bool IsMouseDown(const int &button)
				{
					return glfwGetMouseButton(button);
				}
			};
		};
	};
};
#endif /*INPUT_H_*/

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

#include "Engine/GLFW/glfw.h"
#include <ostream>
#include <iostream>
#include <memory>
#include <vector>

namespace OE
{
	namespace Input
	{
		class InputListener
		{
		public:
			virtual void OnMouseButton(const int button, const int action){};
			virtual void OnMouseClick(const int startX, const int startY, const int endX, const int endY){};
			virtual void OnMouseMove(const int x, const int y){};
			virtual void OnKeyEvent(const int key, const int action){};
			virtual void OnCharEvent(const int key, const int action){};
		};

		class InputManager
		{
		private:
			static std::vector<InputListener*> _vecInputListeners;

			static int _iMouseX;
			static int _iMouseY;
			static int _iMouseDeltaX;
			static int _iMouseDeltaY;
			static int _iMouseClickStartX;
			static int _iMouseClickStartY;
			static int _iMouseClickEndX;
			static int _iMouseClickEndY;
			static int _iMouseReferenceX;
			static int _iMouseReferenceY;
			static int _iMouseLastPosX;
			static int _iMouseLastPosY;
			static bool _bIsMouseAlreadyDown;
			static bool _rgbKeys[255];
			static bool _bInitialized;
			static bool _bForceMouseToRefPos;
			static bool _bMouseHidden;

			static void GLFWCALL GLFWSetCharEvent(int key, int action);
			static void GLFWCALL GLFWSetKeyEvent(int key, int action);
			static void GLFWCALL GLFWSetMousePos(int x, int y);
			static void GLFWCALL GLFWSetMouseButton(int button, int action);
		public:
			InputManager()
			{
				_bInitialized = false;
			}

			~InputManager()
			{
			}

			static void Initialize();
			static bool IsInitialized(){return _bInitialized;}
			static bool IsMouseForcedToReference(){return _bForceMouseToRefPos;}
			static void SetMouseReferencePos(const int x, const int y)
			{
				_iMouseReferenceX = x;
				_iMouseReferenceY = y;
			}

			static void SetForceToMouseReference(bool forceReference)
			{
				_bForceMouseToRefPos = forceReference;
			}

			static void Update(double dt);

			static void AddInputListener(InputListener* inputListener)
			{
				_vecInputListeners.push_back(inputListener);
			}

			static void SetKeyState(const char key, bool state)
			{
				_rgbKeys[(int)key] = state;
			}

			static bool GetKeyState(const char key)
			{
				return _rgbKeys[(int)key];
			}

			static void SetMousePos(const int xPos, const int yPos);

			static void SetMouseX(const int xPos)
			{
				glfwSetMousePos(xPos, _iMouseY);
			}

			static void SetMouseY(const int yPos)
			{
				glfwSetMousePos(_iMouseX, yPos);
			}	

			static void SetCursorVisible(bool con)
			{
				if(con)
					glfwEnable(GLFW_MOUSE_CURSOR);
				else
					glfwDisable(GLFW_MOUSE_CURSOR);
				_bMouseHidden = !con;
			}

			static void GetMousePos(int *xPos, int *yPos);

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

			static bool IsCursorVisible()
			{
				return !_bMouseHidden;
			}

			static bool IsMouseDown(const int button)
			{
				return glfwGetMouseButton(button);
			}
		};
	};
};

#endif /*INPUT_H_*/

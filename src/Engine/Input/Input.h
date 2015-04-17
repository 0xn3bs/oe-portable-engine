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

#include <GLFW\glfw3.h>
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
			virtual void OnMouseMove(const double x, const double y){};
			virtual void OnKeyEvent(const int key, const int action, const int mods){};
			virtual void OnCharEvent(const int codepoint){};
		};

		class InputManager
		{
		private:
			static std::vector<InputListener*> _vecInputListeners;

			static GLFWwindow* _window;

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
			static bool _rgbKeys[512];
			static bool _bInitialized;
			static bool _bForceMouseToRefPos;
			static bool _bMouseHidden;

			static void GLFWSetCharEvent(GLFWwindow* window, unsigned int codepoint);
			static void GLFWSetKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void GLFWSetCursorPos(GLFWwindow* window, double x, double y);
			static void GLFWSetMouseButton(GLFWwindow* window, int button, int action, int mods);

		public:
			InputManager()
			{
				_bInitialized = false;
			}

			~InputManager()
			{
			}

			static void Initialize(GLFWwindow* window);

			static void CopyToClipboard(const char* value);
			static const char* GetFromClipboard();

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

			static int GetMouseButton(int button)
			{
				return glfwGetMouseButton(_window, button);
			}

			static int GetKey(int key)
			{
				return glfwGetKey(_window, key);
			}

			static void SetKeyState(const char key, bool state)
			{
				_rgbKeys[(int)key] = state;
			}

			static bool GetKeyState(const char key)
			{
				return _rgbKeys[(int)key];
			}

			static void SetMousePos(const double xPos, const double yPos);

			static void SetMouseX(const double xPos)
			{
				glfwSetCursorPos(_window, xPos, _iMouseY);
			}

			static void SetMouseY(const double yPos)
			{
				glfwSetCursorPos(_window, _iMouseX, yPos);
			}	

			static void SetCursorDisabled(bool con)
			{
				if(con)
					glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				else
					glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

				_bMouseHidden = !con;
			}

			static void GetMousePos(double *xPos, double *yPos);

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
				return glfwGetMouseButton(_window, button);
			}
		};
	};
};

#endif /*INPUT_H_*/

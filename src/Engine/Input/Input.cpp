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
#include "Input.h"

#ifdef _WIN32
#include <Windows.h>
#endif

GLFWwindow* OE::Input::InputManager::_window = NULL;
OE::Input::InputManager g_inputManager;
bool OE::Input::InputManager::_rgbKeys[512];
int OE::Input::InputManager::_iMouseX=0;
int OE::Input::InputManager::_iMouseY=0;
int OE::Input::InputManager::_iMouseDeltaX=0;
int OE::Input::InputManager::_iMouseDeltaY=0;
int OE::Input::InputManager::_iMouseClickStartX=0;
int OE::Input::InputManager::_iMouseClickStartY=0;
int OE::Input::InputManager::_iMouseClickEndX=0;
int OE::Input::InputManager::_iMouseClickEndY=0;
int OE::Input::InputManager::_iMouseReferenceX=0;
int OE::Input::InputManager::_iMouseReferenceY=0;
int OE::Input::InputManager::_iMouseLastPosX=0;
int OE::Input::InputManager::_iMouseLastPosY=0;
bool OE::Input::InputManager::_bIsMouseAlreadyDown=false;
bool OE::Input::InputManager::_bInitialized=false;
bool OE::Input::InputManager::_bForceMouseToRefPos=false;
bool OE::Input::InputManager::_bMouseHidden=false;

std::vector<OE::Input::InputListener*> OE::Input::InputManager::_vecInputListeners = std::vector<OE::Input::InputListener*>();

void OE::Input::InputManager::GLFWSetCharEvent(GLFWwindow* window, unsigned int codepoint)
{
	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
	{
		_vecInputListeners.at(i)->OnCharEvent(codepoint);
	}
}

void OE::Input::InputManager::GLFWSetKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	_rgbKeys[key] = (action == GLFW_PRESS) || (action == GLFW_REPEAT) ? 1 : 0;

	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
	{
		_vecInputListeners.at(i)->OnKeyEvent(key,action,mods);
	}
}

void OE::Input::InputManager::GLFWSetCursorPos(GLFWwindow* window, double x, double y)
{
	_iMouseX = x;
	_iMouseY = y;
	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
	{
		_vecInputListeners.at(i)->OnMouseMove(x, y);
	}
}

void OE::Input::InputManager::GLFWSetMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	bool click = false;
	if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		_bIsMouseAlreadyDown = true;
		_iMouseClickStartX = _iMouseX;
		_iMouseClickStartY = _iMouseY;
	}
	if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE && _bIsMouseAlreadyDown)
	{
		_bIsMouseAlreadyDown = false;
		_iMouseClickEndX = _iMouseX;
		_iMouseClickEndY = _iMouseY;
		click = true;
	}
	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
	{
		_vecInputListeners.at(i)->OnMouseButton(button, action);
		if(click)
			_vecInputListeners.at(i)->OnMouseClick(_iMouseClickStartX, _iMouseClickStartY, _iMouseClickEndX, _iMouseClickEndY);
	}
}

void OE::Input::InputManager::CopyToClipboard(const char *value)
{
	glfwSetClipboardString(_window, value);
}

const char* OE::Input::InputManager::GetFromClipboard()
{
	return glfwGetClipboardString(_window);
}

void OE::Input::InputManager::Initialize(GLFWwindow* window)
{
	_window = window;
	glfwSetCharCallback(_window, GLFWSetCharEvent);
	glfwSetKeyCallback(_window, GLFWSetKeyEvent);
	glfwSetCursorPosCallback(_window, GLFWSetCursorPos);
	glfwSetMouseButtonCallback(_window, GLFWSetMouseButton);
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	_bInitialized = true;
}

void OE::Input::InputManager::Update(double dt)
{
	double x, y;
	GetMousePos(&x, &y);

	if(_bForceMouseToRefPos && _bMouseHidden)
	{
		_iMouseDeltaX = _iMouseReferenceX - x;
		_iMouseDeltaY = _iMouseReferenceY - y;

		//SetMousePos(_iMouseReferenceX, _iMouseReferenceY);

		_iMouseLastPosX = _iMouseX;
		_iMouseLastPosY = _iMouseY;
	}
	else
	{
		_iMouseDeltaX = _iMouseLastPosX - x;
		_iMouseDeltaY = _iMouseLastPosY - y;
		_iMouseLastPosX = x;
		_iMouseLastPosY = y;
	}
	glfwPollEvents();
}

void OE::Input::InputManager::SetMousePos(const double xPos, const double yPos)
{
	//glfwSetCursorPos(_window, xPos, yPos);
}

void OE::Input::InputManager::GetMousePos(double *xPos, double *yPos)
{
	glfwGetCursorPos(_window, xPos, yPos);
}

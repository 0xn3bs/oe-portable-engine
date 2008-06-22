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

OE::Input::InputManager g_inputManager;
bool OE::Input::InputManager::_rgbKeys[255];
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
bool OE::Input::InputManager::_bIsMouseAlreadyDown=false;
bool OE::Input::InputManager::_bInitialized=false;
bool OE::Input::InputManager::_bForceMouseToRefPos=false;

std::vector<OE::Input::InputListener*> OE::Input::InputManager::_vecInputListeners = std::vector<OE::Input::InputListener*>();

void GLFWCALL OE::Input::InputManager::GLFWSetKeyEvent(int key, int action)
{
	_rgbKeys[key] = (bool)action;
	for(unsigned int i = 0; i < _vecInputListeners.size(); i++)
	{
		_vecInputListeners.at(i)->OnKeyEvent(key,action);
	}
}

void GLFWCALL OE::Input::InputManager::GLFWSetMousePos(int x, int y)
{
	for(unsigned int i = 0; i < _vecInputListeners.size(); i++)
	{
		_vecInputListeners.at(i)->OnMouseMove(x, y);
	}
}

void OE::Input::InputManager::GLFWSetMouseButton(int button, int action)
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
	for(unsigned int i = 0; i < _vecInputListeners.size(); i++)
	{
		_vecInputListeners.at(i)->OnMouseButton(button, action);
		if(click)
			_vecInputListeners.at(i)->OnMouseClick(_iMouseClickStartX, _iMouseClickStartY, _iMouseClickEndX, _iMouseClickEndY);
	}
}

void OE::Input::InputManager::Initialize()
{
	glfwSetKeyCallback(GLFWSetKeyEvent);
	glfwSetMousePosCallback(GLFWSetMousePos);
	glfwSetMouseButtonCallback(GLFWSetMouseButton);
	_bInitialized = true;
}

void OE::Input::InputManager::Update(double &dt)
{
	int x, y;
	glfwGetMousePos(&x, &y);

	if(_bForceMouseToRefPos)
	{
		_iMouseDeltaX = _iMouseReferenceX - x;
		_iMouseDeltaY = _iMouseReferenceY - y;

		//	Only set the mouse position back if there has been a change.
		if(_iMouseDeltaX != 0 || _iMouseDeltaY != 0)
		{
			_iMouseX = _iMouseReferenceX;
			_iMouseY = _iMouseReferenceY;
			glfwSetMousePos(_iMouseReferenceX, _iMouseReferenceY);
		}
	}
	else
	{
		_iMouseDeltaX = x - _iMouseX;
		_iMouseDeltaY = y - _iMouseY;
		_iMouseX = x;
		_iMouseY = y;
	}
}

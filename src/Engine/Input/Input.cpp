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

void GLFWCALL OE::Input::InputManager::GLFWSetCharEvent(int key, int action)
{
	_rgbKeys[key] = action == GLFW_PRESS ? 1 : 0;
	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
	{
		_vecInputListeners.at(i)->OnCharEvent(key,action);
	}
}

void GLFWCALL OE::Input::InputManager::GLFWSetKeyEvent(int key, int action)
{
	_rgbKeys[key] = action == GLFW_PRESS ? 1 : 0;
	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
	{
		_vecInputListeners.at(i)->OnKeyEvent(key,action);
	}
}

void GLFWCALL OE::Input::InputManager::GLFWSetMousePos(int x, int y)
{
	_iMouseX = x;
	_iMouseY = y;
	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
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
	for(unsigned int i = 0; i < _vecInputListeners.size(); ++i)
	{
		_vecInputListeners.at(i)->OnMouseButton(button, action);
		if(click)
			_vecInputListeners.at(i)->OnMouseClick(_iMouseClickStartX, _iMouseClickStartY, _iMouseClickEndX, _iMouseClickEndY);
	}
}

void OE::Input::InputManager::CopyToClipboard(const char *value)
{
#ifdef _WIN32
	if(OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		char * buffer = NULL;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(value)+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, value);
		buffer[strlen(value)] = 0;
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
#endif
}

const char* OE::Input::InputManager::GetFromClipboard()
{
#ifdef _WIN32
	if(OpenClipboard(NULL))
	{
		HGLOBAL clipbuffer;
		char * buffer = NULL;
		HANDLE hData = GetClipboardData( CF_TEXT );
		buffer = (char*)GlobalLock( hData );
		GlobalUnlock( hData );
		CloseClipboard();
		return buffer;
	}
	else
		return NULL;
#endif
}

void OE::Input::InputManager::Initialize()
{
	glfwSetCharCallback(GLFWSetCharEvent);
	glfwSetKeyCallback(GLFWSetKeyEvent);
	glfwSetMousePosCallback(GLFWSetMousePos);
	glfwSetMouseButtonCallback(GLFWSetMouseButton);
	glfwEnable(GLFW_KEY_REPEAT);
	glfwEnable(GLFW_STICKY_KEYS);
	glfwEnable(GLFW_SYSTEM_KEYS);
	_bInitialized = true;
}

void OE::Input::InputManager::Update(double dt)
{
	int x, y;
	GetMousePos(&x, &y);

	if(_bForceMouseToRefPos && _bMouseHidden)
	{
		_iMouseDeltaX = _iMouseReferenceX - x;
		_iMouseDeltaY = _iMouseReferenceY - y;

		SetMousePos(_iMouseReferenceX, _iMouseReferenceY);

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
}

void OE::Input::InputManager::SetMousePos(const int xPos, const int yPos)
{
	glfwSetMousePos(xPos, yPos);
}

void OE::Input::InputManager::GetMousePos(int *xPos, int *yPos)
{
	glfwGetMousePos(xPos, yPos);
}

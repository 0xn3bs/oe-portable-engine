/*****************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
*****************************************************************************************/
#include "WindowManager.h"
#include <iostream>
#include "Engine/Input/Input.h"

void Odorless::Engine::UI::Windows::WindowManager::OnMouseButton(const int key, const int action)
{
	int iMouseX, iMouseY;
	Odorless::Engine::Input::InputManager::GetMousePos(&iMouseX, &iMouseY);
	if(key == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		for(int i = 0; i < _vecWindows.size(); i++)
		{
			Odorless::Engine::UI::Windows::Window* tempWin = _vecWindows.at(i);
			if(tempWin->_bIsDragging)
				tempWin->_bIsDragging = false;
		}
	}

	if(key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		for(int i = 0; i < _vecWindows.size(); i++)
		{
			Odorless::Engine::UI::Windows::Window* tempWin = _vecWindows.at(i);
			bool bCanPick = CanPick(i, iMouseX, iMouseY);

			if(bCanPick&&!tempWin->_bHasFocus)
				SetFocus(i);

			if(bCanPick&&tempWin->IsOverTitleBar(iMouseX, iMouseY))
				tempWin->_bIsDragging = true;
		}
		std::cout << std::endl;
	}
	UpdateFocus();
}

void Odorless::Engine::UI::Windows::WindowManager::Update(const float &dt)
{
	for(int i = 0; i < _vecWindows.size(); i++)
	{
		Odorless::Engine::UI::Windows::Window* tempWin = _vecWindows.at(i);

		//	Window Bounds Checking, we want the user to always have a way of grabbing a window.
		if(tempWin->_2fPosition[1] < -(tempWin->_2fDimensions[1]*tempWin->_fTitleBarY)+5)
			tempWin->_2fPosition[1] = -(tempWin->_2fDimensions[1]*tempWin->_fTitleBarY)+5;
		if(tempWin->_2fPosition[1] > _iWinHeight-5)
			tempWin->_2fPosition[1] = _iWinHeight-5;
		if(tempWin->_2fPosition[0] < -tempWin->_2fDimensions[0]+5)
			tempWin->_2fPosition[0] = -tempWin->_2fDimensions[0]+5;
		if(tempWin->_2fPosition[0] > _iWinWidth-5)
			tempWin->_2fPosition[0] = _iWinWidth-5;

		if(tempWin->_bIsDragging)
		{
			tempWin->_2fPosition[0] += Odorless::Engine::Input::InputManager::GetMouseDeltaX();
			tempWin->_2fPosition[1] += Odorless::Engine::Input::InputManager::GetMouseDeltaY();
		}

		if(Engine::Input::InputManager::GetMouseX() < 0)
		{
			tempWin->_bIsDragging = false;
			continue;
		}

		if(Engine::Input::InputManager::GetMouseX() > _iWinWidth)
		{
			tempWin->_bIsDragging = false;
			continue;
		}

		if(Engine::Input::InputManager::GetMouseY() < 0)
		{
			tempWin->_bIsDragging = false;
			continue;
		}

		if(Engine::Input::InputManager::GetMouseY() > _iWinHeight)
		{
			tempWin->_bIsDragging = false;
			continue;
		}
	}
}

void Odorless::Engine::UI::Windows::WindowManager::UpdateWin(const int width, const int height)
{
	_iWinWidth = width;
	_iWinHeight = height;
}

void Odorless::Engine::UI::Windows::WindowManager::Render(const float &dt)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* tempWin = _vecWindows.at(i);
		glMatrixMode( GL_PROJECTION );
		glViewport(tempWin->_2fPosition[0], _iWinHeight-tempWin->_2fDimensions[0]-tempWin->_2fPosition[1], tempWin->_2fDimensions[0], tempWin->_2fDimensions[1]);
		tempWin->Render(dt);
	}
}

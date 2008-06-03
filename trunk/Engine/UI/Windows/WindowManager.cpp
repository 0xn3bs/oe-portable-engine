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

			for(int j = 0; j < tempWin->_vecWidgets.size(); j++)
			{
				bool bIsOver = tempWin->_vecWidgets[j]->IsOver(iMouseX, iMouseY) && bCanPick;
				if(bIsOver)
					tempWin->_vecWidgets[j]->OnMouseButton(key, action);
			}

			if(bCanPick&&!tempWin->_bHasFocus)
				SetFocus(i);

			if(bCanPick&&tempWin->IsOverTitleBar(iMouseX, iMouseY))
				tempWin->_bIsDragging = true;
		}
	}

	//	Setting a new focused window changes the order of the windows.
	UpdateFocus();
}
void Odorless::Engine::UI::Windows::WindowManager::OnMouseClick(const int startX, const int startY, const int endX, const int endY)
{
	for(int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);

		for(int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			bool startIsOver = winTemp->_vecWidgets[j]->IsOver(startX, startY) && CanPick(i, startX, startY);
			bool endIsOver = winTemp->_vecWidgets[j]->IsOver(endX, endY) && CanPick(i, endX, endY);
			if(startIsOver&&endIsOver)
				winTemp->_vecWidgets[j]->OnMouseClick();
		}
	}
}
void Odorless::Engine::UI::Windows::WindowManager::OnMouseMove(const int x, const int y)
{
	for(int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);
		for(int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			bool isOver = winTemp->_vecWidgets[j]->IsOver(x, y) && CanPick(i, x, y);

			if(isOver && !winTemp->_vecWidgets[j]->_bIsOver)
			{
				winTemp->_vecWidgets[j]->_bIsOver = true;
				winTemp->_vecWidgets[j]->OnMouseOver();
			}
			else
				if(!isOver && winTemp->_vecWidgets[j]->_bIsOver)
				{
					winTemp->_vecWidgets[j]->_bIsOver = false;
					winTemp->_vecWidgets[j]->OnMouseOut();
				}
		}
	}
}
bool Odorless::Engine::UI::Windows::WindowManager::CanPick(const unsigned int &index, const unsigned int &x, const unsigned int &y)
{
	Window* winTemp = _vecWindows.at(index);
	bool bOver = winTemp->IsOver(x, y);
	for(int i = index+1; i < _vecWindows.size(); i++)
	{
		if((_vecWindows[i]->IsOver(x,y) || _vecWindows[i]->IsOverTitleBar(x,y)))
			return false;
	}
	return bOver;
}
void Odorless::Engine::UI::Windows::WindowManager::Update(const float &dt)
{
	for(int i = 0; i < _vecWindows.size(); i++)
	{
		Odorless::Engine::UI::Windows::Window* tempWin = _vecWindows.at(i);

		//	Window Bounds Checking, we want the user to always have a way of grabbing a window.
		if(tempWin->_2fPosition[1] < -tempWin->_fTitleBarY+5)
			tempWin->_2fPosition[1] = -tempWin->_fTitleBarY+5;
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
		glScalef(tempWin->_fScaleX,tempWin->_fScaleY,1);
		glViewport(tempWin->_2fPosition[0], _iWinHeight-tempWin->_2fDimensions[0]-tempWin->_2fPosition[1], tempWin->_2fDimensions[0], tempWin->_2fDimensions[1]);
		tempWin->Render(dt);
		glScalef(1/tempWin->_fScaleX,1/tempWin->_fScaleY,1);
	}
}

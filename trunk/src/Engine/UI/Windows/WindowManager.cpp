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

void OEngine::UI::Windows::WindowManager::OnMouseButton(const int key, const int action)
{
	int iMouseX, iMouseY;
	OEngine::Input::InputManager::GetMousePos(&iMouseX, &iMouseY);

	if(key == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		for(int i = 0; i < _vecWindows.size(); i++)
		{
			OEngine::UI::Windows::Window* tempWin = _vecWindows.at(i);
			if(tempWin->_bIsDragging)
				tempWin->_bIsDragging = false;
		}
	}

	if(key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		for(unsigned int i = 0; i < _vecWindows.size(); i++)
		{
			OEngine::UI::Windows::Window* tempWin = _vecWindows.at(i);
			bool bCanPick = CanPick(i, iMouseX, iMouseY);

			for(unsigned int j = 0; j < tempWin->_vecWidgets.size(); j++)
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
void OEngine::UI::Windows::WindowManager::OnMouseClick(const int startX, const int startY, const int endX, const int endY)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);

		for(unsigned int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			bool startIsOver = winTemp->_vecWidgets[j]->IsOver(startX, startY) && CanPick(i, startX, startY);
			bool endIsOver = winTemp->_vecWidgets[j]->IsOver(endX, endY) && CanPick(i, endX, endY);
			if(startIsOver&&endIsOver)
				winTemp->_vecWidgets[j]->OnMouseClick();
		}
	}
}

void OEngine::UI::Windows::WindowManager::OnMouseMove(const int x, const int y)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);
		for(unsigned int j = 0; j < winTemp->_vecWidgets.size(); j++)
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
bool OEngine::UI::Windows::WindowManager::CanPick(const unsigned int &index, const unsigned int &x, const unsigned int &y)
{
	Window* winTemp = _vecWindows.at(index);
	bool bOver = winTemp->IsOver(x, y);
	for(unsigned int i = index+1; i < _vecWindows.size(); i++)
	{
		if((_vecWindows[i]->IsOver(x,y) || _vecWindows[i]->IsOverTitleBar(x,y)))
			return false;
	}
	return bOver;
}
void OEngine::UI::Windows::WindowManager::Update(const float &dt)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		OEngine::UI::Windows::Window* winTemp = _vecWindows.at(i);

		//	Window Bounds Checking, we want the user to always have a way of grabbing a window.
		if(winTemp->_2fPosition[1] < -winTemp->_fTitleBarY+5)
			winTemp->_2fPosition[1] = -winTemp->_fTitleBarY+5;
		if(winTemp->_2fPosition[1] > _iWinHeight-5)
			winTemp->_2fPosition[1] = _iWinHeight-5;
		if(winTemp->_2fPosition[0] < -winTemp->_2fDimensions[0]+5)
			winTemp->_2fPosition[0] = -winTemp->_2fDimensions[0]+5;
		if(winTemp->_2fPosition[0] > _iWinWidth-5)
			winTemp->_2fPosition[0] = _iWinWidth-5;

		if(winTemp->_bIsDragging)
		{
			winTemp->_2fPosition[0] += OEngine::Input::InputManager::GetMouseDeltaX();
			winTemp->_2fPosition[1] += OEngine::Input::InputManager::GetMouseDeltaY();
		}

		if(OEngine::Input::InputManager::GetMouseX() < 0)
		{
			winTemp->_bIsDragging = false;
			continue;
		}

		if(OEngine::Input::InputManager::GetMouseX() > _iWinWidth)
		{
			winTemp->_bIsDragging = false;
			continue;
		}

		if(OEngine::Input::InputManager::GetMouseY() < 0)
		{
			winTemp->_bIsDragging = false;
			continue;
		}

		if(OEngine::Input::InputManager::GetMouseY() > _iWinHeight)
		{
			winTemp->_bIsDragging = false;
			continue;
		}
	}
}
void OEngine::UI::Windows::WindowManager::UpdateWin(const int width, const int height)
{
	_iWinWidth = width;
	_iWinHeight = height;
}
void OEngine::UI::Windows::WindowManager::Render(const float &dt)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);
		glMatrixMode( GL_PROJECTION );
		glScalef(winTemp->_fScaleX,winTemp->_fScaleY,1);
		glViewport(winTemp->_2fPosition[0], _iWinHeight-winTemp->_2fDimensions[0]-winTemp->_2fPosition[1], winTemp->_2fDimensions[0], winTemp->_2fDimensions[1]);
		winTemp->Render(dt);
		glScalef(1/winTemp->_fScaleX,1/winTemp->_fScaleY,1);
	}
}

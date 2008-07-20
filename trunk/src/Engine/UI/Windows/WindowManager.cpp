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

void OE::UI::Windows::WindowManager::OnMouseButton(const int key, const int action)
{
	int iMouseX, iMouseY;
	OE::Input::InputManager::GetMousePos(&iMouseX, &iMouseY);

	if(key == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		for(unsigned int i = 0; i < _vecWindows.size(); i++)
		{
			OE::UI::Windows::Window* tempWin = _vecWindows.at(i);

			bool bCanPick = CanPick(i, iMouseX, iMouseY);

			for(unsigned int j = 0; j < tempWin->_vecWidgets.size(); j++)
			{
				bool bIsOver = tempWin->_vecWidgets[j]->IsOver(static_cast<float>(iMouseX), static_cast<float>(iMouseY)) && bCanPick;
				if(bIsOver)
				{
					tempWin->_vecWidgets[j]->OnMouseButton(key, action);
				}
			}

			if(tempWin->_bIsDragging)
			{
				tempWin->_bIsDragging = false;
			}
		}
	}

	if(key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		for(unsigned int i = 0; i < _vecWindows.size(); i++)
		{
			OE::UI::Windows::Window* tempWin = _vecWindows.at(i);
			bool bCanPick = CanPick(i, iMouseX, iMouseY);

			for(unsigned int j = 0; j < tempWin->_vecWidgets.size(); j++)
			{
				tempWin->_vecWidgets[j]->_bHasFocus = false;
				bool bIsOver = tempWin->_vecWidgets[j]->IsOver(static_cast<float>(iMouseX), static_cast<float>(iMouseY)) && bCanPick;
				if(bIsOver)
				{
					tempWin->_vecWidgets[j]->_bHasFocus = true;
					tempWin->_vecWidgets[j]->OnMouseButton(key, action);
				}
			}

			if(bCanPick&&!tempWin->_bHasFocus)
				SetFocus(i);

			if(bCanPick&&tempWin->IsOverTitleBar(static_cast<float>(iMouseX), static_cast<float>(iMouseY)))
				tempWin->_bIsDragging = true;
		}
	}

	//	Setting a new focused window changes the order of the windows.
	UpdateFocus();
}

void OE::UI::Windows::WindowManager::OnMouseClick(const int startX, const int startY, const int endX, const int endY)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);

		for(unsigned int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			bool startIsOver = winTemp->_vecWidgets[j]->IsOver(static_cast<float>(startX), static_cast<float>(startY)) && CanPick(i, startX, startY);
			bool endIsOver = winTemp->_vecWidgets[j]->IsOver(static_cast<float>(endX), static_cast<float>(endY)) && CanPick(i, endX, endY);
			if(startIsOver&&endIsOver)
			{
				winTemp->_vecWidgets[j]->OnMouseClick();
			}
		}
	}
}

void OE::UI::Windows::WindowManager::OnCharEvent(const int key, const int action)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);
		winTemp->OnCharEvent(key, action);
		for(unsigned int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			winTemp->_vecWidgets[j]->OnCharEvent(key, action);
		}
	}
}

void OE::UI::Windows::WindowManager::OnKeyEvent(const int key, const int action)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);
		winTemp->OnKeyEvent(key, action);
		for(unsigned int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			winTemp->_vecWidgets[j]->OnKeyEvent(key, action);
		}
	}
}

void OE::UI::Windows::WindowManager::OnMouseMove(const int x, const int y)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		Window* winTemp = _vecWindows.at(i);
		for(unsigned int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			bool isOver = winTemp->_vecWidgets[j]->IsOver(static_cast<float>(x), static_cast<float>(y)) && CanPick(i, x, y);
			
			if(isOver || winTemp->_vecWidgets[j]->_bHasFocus)
				winTemp->_vecWidgets[j]->OnMouseMove(x,y);

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

bool OE::UI::Windows::WindowManager::CanPick(const unsigned int index, const unsigned int x, const unsigned int y)
{
	Window* winTemp = _vecWindows.at(index);
	bool bOver = winTemp->IsOver(static_cast<float>(x), static_cast<float>(y));
	for(unsigned int i = index+1; i < _vecWindows.size(); i++)
	{
		if((_vecWindows[i]->IsOver(static_cast<float>(x),static_cast<float>(y)) || _vecWindows[i]->IsOverTitleBar(static_cast<float>(x),static_cast<float>(y))))
			return false;
	}
	return bOver;
}

void OE::UI::Windows::WindowManager::Update(const float dt)
{
	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		OE::UI::Windows::Window* winTemp = _vecWindows.at(i);

		for(unsigned int j = 0; j < winTemp->_vecWidgets.size(); j++)
		{
			winTemp->_vecWidgets[j]->Update(dt);
		}

		//	Window Bounds Checking, we want the user to always have a way of grabbing a window.
		if(winTemp->_v2fPosition.y < -winTemp->_fTitleBarY+5)
			winTemp->_v2fPosition.y = -winTemp->_fTitleBarY+5;
		if(winTemp->_v2fPosition.y > _iWinHeight-5)
			winTemp->_v2fPosition.y = static_cast<float>(_iWinHeight-5);
		if(winTemp->_v2fPosition.x < -winTemp->_v2fDimensions.x+5)
			winTemp->_v2fPosition.x = -winTemp->_v2fDimensions.x+5;
		if(winTemp->_v2fPosition.x > _iWinWidth-5)
			winTemp->_v2fPosition.x = static_cast<float>(_iWinWidth-5);

		if(winTemp->_bIsDragging)
		{
			winTemp->_v2fPosition.x -= OE::Input::InputManager::GetMouseDeltaX();
			winTemp->_v2fPosition.y -= OE::Input::InputManager::GetMouseDeltaY();
		}

		if(OE::Input::InputManager::GetMouseX() < 0)
		{
			winTemp->_bIsDragging = false;
			continue;
		}

		if(OE::Input::InputManager::GetMouseX() > _iWinWidth)
		{
			winTemp->_bIsDragging = false;
			continue;
		}

		if(OE::Input::InputManager::GetMouseY() < 0)
		{
			winTemp->_bIsDragging = false;
			continue;
		}

		if(OE::Input::InputManager::GetMouseY() > _iWinHeight)
		{
			winTemp->_bIsDragging = false;
			continue;
		}
	}
}

void OE::UI::Windows::WindowManager::UpdateWin(const int width, const int height)
{
	_iWinWidth = width;
	_iWinHeight = height;
}

void OE::UI::Windows::WindowManager::Render(const float dt)
{
  	glMatrixMode(GL_PROJECTION);
  	glPushMatrix();
  	glLoadIdentity();
  	glOrtho(0, 1, 1, 0, -1, 1);
  	glMatrixMode(GL_MODELVIEW);
  	glPushMatrix();
  	glLoadIdentity();
  	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
  	glPopMatrix();
  	glMatrixMode(GL_PROJECTION);

	for(unsigned int i = 0; i < _vecWindows.size(); i++)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);
		glColor4ub(255,255,255,255);
		Window* winTemp = _vecWindows.at(i);
		glViewport(winTemp->_v2fPosition.x, OE::Base::_iWindowHeight-winTemp->_v2fDimensions.y-winTemp->_v2fPosition.y, winTemp->_v2fDimensions.x, winTemp->_v2fDimensions.y);
		winTemp->Render(dt);
	}

  	glPopMatrix();
}

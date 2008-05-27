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

void Odorless::Engine::UI::Windows::WindowManager::Update(const float &dt)
{
	/*
	const int iMouseX = _inputManager->GetMouseX();
	const int iMouseY = _inputManager->GetMouseY();

	for(int i = 0; i < _vecWindows.size(); i++)
	{
	Odorless::Engine::UI::Windows::Window *tempWin = _vecWindows.at(i); 
	tempWin->Update(dt);

	if(CanPick(i, iMouseX, iMouseY))
	SetFocus(i);
	}
	*/
	const int iMouseX = _inputManager->GetMouseX();
	const int iMouseY = _inputManager->GetMouseY();

	for(int i = 0; i < _vecWindows.size(); i++)
	{
		Odorless::Engine::UI::Windows::Window *tempWin = _vecWindows.at(i); 
		tempWin->Update(dt);

		//	Window Bounds Checking, we want the user to always have a way of grabbing a window.
		if(tempWin->_2fPosition[1] < -(tempWin->_2fDimensions[1]*tempWin->_fTitleBarY)+5)
			tempWin->_2fPosition[1] = -(tempWin->_2fDimensions[1]*tempWin->_fTitleBarY)+5;
		if(tempWin->_2fPosition[1] > _iWinHeight-5)
			tempWin->_2fPosition[1] = _iWinHeight-5;
		if(tempWin->_2fPosition[0] < -tempWin->_2fDimensions[0]+5)
			tempWin->_2fPosition[0] = -tempWin->_2fDimensions[0]+5;
		if(tempWin->_2fPosition[0] > _iWinWidth-5)
			tempWin->_2fPosition[0] = _iWinWidth-5;

		bool bOthersDragging = false;
		for(int j = 0; j < _vecWindows.size(); j++)
		{
			if(_vecWindows.at(j)->_bIsDragging && j != i)
				bOthersDragging = true;
		}

		bool bCanPick = CanPick(i, iMouseX, iMouseY);

		if(((tempWin->IsOverTitleBar(iMouseX, iMouseY) && bCanPick) || tempWin->_bIsDragging) && !bOthersDragging)
		{
			if(_inputManager->GetMouseX() < 0)
			{
				tempWin->_bIsDragging = false;
				break;
			}

			if(_inputManager->GetMouseX() > _iWinWidth)
			{
				tempWin->_bIsDragging = false;
				break;
			}

			if(_inputManager->GetMouseY() < 0)
			{
				tempWin->_bIsDragging = false;
				break;
			}

			if(_inputManager->GetMouseY() > _iWinHeight)
			{
				tempWin->_bIsDragging = false;
				break;
			}

			if(_inputManager->IsMouseDown(0))
			{
				SetFocus(i);
				tempWin->_bIsDragging = true;
				tempWin->_2fPosition[0] += _inputManager->GetMouseDeltaX();
				tempWin->_2fPosition[1] += _inputManager->GetMouseDeltaY();
			}
			else
				tempWin->_bIsDragging = false;
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

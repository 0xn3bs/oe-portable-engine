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

void Odorless::Engine::UI::Windows::WindowManager::Update(const float &dt)
{
	for(int i = 0; i < _vecWindows.size(); i++)
	{
		_vecWindows.at(i)->Update(dt);
	}
}

void Odorless::Engine::UI::Windows::WindowManager::UpdateWin(const int width, const int height)
{
	_iWinWidth = width;
	_iWinHeight = height;
}

void Odorless::Engine::UI::Windows::WindowManager::Render(const float &dt)
{
	for(int i = 0; i < _vecWindows.size(); i++)
	{
		Window* tempWin = _vecWindows.at(i);
		glMatrixMode( GL_PROJECTION );
		glViewport(tempWin->_2fPosition[0], _iWinHeight-tempWin->_2fDimensions[0]-tempWin->_2fPosition[1], tempWin->_2fDimensions[0], tempWin->_2fDimensions[1]);
		tempWin->Render(dt);
	}
}

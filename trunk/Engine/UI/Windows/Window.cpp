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
#include "Window.h"

Odorless::Engine::UI::Windows::Window::Window(float x, float y, float width, float height)
{
	_2fPosition[0] = x;
	_2fPosition[1] = y;
	_2fDimensions[0] = width;
	_2fDimensions[1] = height;
}

Odorless::Engine::UI::Windows::Window::~Window()
{

}

bool Odorless::Engine::UI::Windows::Window::IsOver(float pos[2])
{
	bool x = pos[0] > _2fPosition[0] && pos[0] < _2fPosition[0] + _2fDimensions[0];
	bool y = pos[1] > _2fPosition[1] && pos[1] < _2fPosition[1] + _2fDimensions[1];
	return x && y;
}

bool Odorless::Engine::UI::Windows::Window::IsOverTitleBar(float pos[2])
{
	return false;
}

void Odorless::Engine::UI::Windows::Window::AddWidget(
		const UI::Widgets::Widget &widget)
{
	
}

void Odorless::Engine::UI::Windows::Window::RemoveWidget(
		const unsigned int &index)
{

}

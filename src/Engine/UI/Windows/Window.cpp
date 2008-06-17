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

OE::UI::Windows::Window::Window()
{
	_2fPosition[0] = 0;
	_2fPosition[1] = 0;
	_2fDimensions[0] = 0;
	_2fDimensions[1] = 0;
	_fTitleBarY = 0;
	_bIsDragging = false;
}
OE::UI::Windows::Window::Window(const float &x, const float &y, const float &width, const float &height)
{
	_2fPosition[0] = x;
	_2fPosition[1] = y;
	_2fDimensions[0] = width;
	_2fDimensions[1] = height;
	_fScaleX = 1/width;
	_fScaleY = 1/height;
	_fTitleBarY = 0;
	_bIsDragging = false;
}
OE::UI::Windows::Window::~Window()
{

}
bool OE::UI::Windows::Window::IsOver(const float &x, const float &y)
{
	bool isX = x > _2fPosition[0] && x < _2fPosition[0] + _2fDimensions[0];
	bool isY = y > _2fPosition[1] && y < _2fPosition[1] + _2fDimensions[1];
	return isX && isY;
}
bool OE::UI::Windows::Window::IsOverTitleBar(const float &x, const float &y)
{
	bool isX = x > _2fPosition[0] && x < _2fPosition[0] + _2fDimensions[0];
	bool isY = y > _2fPosition[1] && y < _2fPosition[1] + (_fTitleBarY);
	return isX && isY;
}
void OE::UI::Windows::Window::AddWidget(UI::Widgets::Widget *widget)
{
	_vecWidgets.push_back(widget);
}
void OE::UI::Windows::Window::RemoveWidget(
	const unsigned int &index)
{
	_vecWidgets.erase(_vecWidgets.begin()+index);
}

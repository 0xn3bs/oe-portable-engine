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

void Odorless::Engine::Input::Input::SetKeyState(const char key, bool state)
{
	_rgcKeys[key] = state;
}

bool Odorless::Engine::Input::Input::GetKeyState(const char key)
{
	return _rgcKeys[key];
}

const int Odorless::Engine::Input::Input::GetMouseX()
{
	return _iMouseX;
}

const int Odorless::Engine::Input::Input::GetMouseY()
{
	return _iMouseY;
}

void Odorless::Engine::Input::Input::GetMousePos(int *pos)
{
	pos[0] = _iMouseX;
	pos[1] = _iMouseY;
}

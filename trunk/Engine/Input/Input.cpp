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

Odorless::Engine::Input::InputManager g_inputManager;
bool Odorless::Engine::Input::InputManager::_rgcKeys[255];
int Odorless::Engine::Input::InputManager::_iMouseX=0;
int Odorless::Engine::Input::InputManager::_iMouseY=0;
int Odorless::Engine::Input::InputManager::_iMouseDeltaX=0;
int Odorless::Engine::Input::InputManager::_iMouseDeltaY=0;
int Odorless::Engine::Input::InputManager::_iMouseClickStartX=0;
int Odorless::Engine::Input::InputManager::_iMouseClickStartY=0;
int Odorless::Engine::Input::InputManager::_iMouseClickEndX=0;
int Odorless::Engine::Input::InputManager::_iMouseClickEndY=0;
int Odorless::Engine::Input::InputManager::_iMouseReferenceX=0;
int Odorless::Engine::Input::InputManager::_iMouseReferenceY=0;
bool Odorless::Engine::Input::InputManager::_bIsMouseAlreadyDown=false;
bool Odorless::Engine::Input::InputManager::_bInitialized=false;
bool Odorless::Engine::Input::InputManager::_bForceMouseToRefPos=false;

std::vector<Odorless::Engine::Input::InputListener*> Odorless::Engine::Input::InputManager::_vecInputListeners = std::vector<Odorless::Engine::Input::InputListener*>();

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

OE::Input::InputManager g_inputManager;
bool OE::Input::InputManager::_rgcKeys[255];
int OE::Input::InputManager::_iMouseX=0;
int OE::Input::InputManager::_iMouseY=0;
int OE::Input::InputManager::_iMouseDeltaX=0;
int OE::Input::InputManager::_iMouseDeltaY=0;
int OE::Input::InputManager::_iMouseClickStartX=0;
int OE::Input::InputManager::_iMouseClickStartY=0;
int OE::Input::InputManager::_iMouseClickEndX=0;
int OE::Input::InputManager::_iMouseClickEndY=0;
int OE::Input::InputManager::_iMouseReferenceX=0;
int OE::Input::InputManager::_iMouseReferenceY=0;
bool OE::Input::InputManager::_bIsMouseAlreadyDown=false;
bool OE::Input::InputManager::_bInitialized=false;
bool OE::Input::InputManager::_bForceMouseToRefPos=false;

std::vector<OE::Input::InputListener*> OE::Input::InputManager::_vecInputListeners = std::vector<OE::Input::InputListener*>();

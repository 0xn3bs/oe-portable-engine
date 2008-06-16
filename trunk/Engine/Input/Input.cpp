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

OEngine::Input::InputManager g_inputManager;
bool OEngine::Input::InputManager::_rgcKeys[255];
int OEngine::Input::InputManager::_iMouseX=0;
int OEngine::Input::InputManager::_iMouseY=0;
int OEngine::Input::InputManager::_iMouseDeltaX=0;
int OEngine::Input::InputManager::_iMouseDeltaY=0;
int OEngine::Input::InputManager::_iMouseClickStartX=0;
int OEngine::Input::InputManager::_iMouseClickStartY=0;
int OEngine::Input::InputManager::_iMouseClickEndX=0;
int OEngine::Input::InputManager::_iMouseClickEndY=0;
int OEngine::Input::InputManager::_iMouseReferenceX=0;
int OEngine::Input::InputManager::_iMouseReferenceY=0;
bool OEngine::Input::InputManager::_bIsMouseAlreadyDown=false;
bool OEngine::Input::InputManager::_bInitialized=false;
bool OEngine::Input::InputManager::_bForceMouseToRefPos=false;

std::vector<OEngine::Input::InputListener*> OEngine::Input::InputManager::_vecInputListeners = std::vector<OEngine::Input::InputListener*>();

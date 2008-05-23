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
#include "Base.h"
#include <iostream>
#include "Textures/TextureManager.h"

bool Odorless::Engine::Base::_bIsRunning = false;
void (*Odorless::Engine::Base::_pf_Initialize)(void) = 0;
void (*Odorless::Engine::Base::_pf_Update)(double deltaTime) = 0;
void (*Odorless::Engine::Base::_pf_Draw)(double deltaTime) = 0;

///	Function which Intializes the Engine. <b>Call this before anything else is done!</b>
unsigned int Odorless::Engine::Base::InitializeEngine()
{
	glfwInit();

	_bIsRunning = true;
	glfwEnable(GLFW_STICKY_KEYS);

	return 0;
}

/*! \fn Odorless::Engine::Base::OpenWindow(int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits)
\brief Creates, and opens, a Window and buffer that can be rendered to.
\param width The desired width of the window.
\param height The desired height of the window.
\param redbits The number of bits to use in the red color component of the buffer.
\param greenbits The number of bits to use in the green color component of the buffer.
\param bluebits The number of bits to use in the blue color component of the buffer.
\param alphabits The number of bits to use for the alpha buffer.
\param depthbits The number of bits to use for the depth buffer.
\param stencilbits The number of bits to use for the stencil buffer.
*/
int Odorless::Engine::Base::OpenWindow(int width, int height, int redbits,
										  int greenbits, int bluebits, int alphabits, int depthbits,
										  int stencilbits)
{
	int window = glfwOpenWindow(width,
		height,
		redbits,
		greenbits,
		bluebits,
		alphabits,
		depthbits,
		stencilbits,
		GLFW_WINDOW);

	if (!window)
		Dispose();

	return 0;
}

void Odorless::Engine::Base::Initialize()
{
	if (_pf_Initialize != 0)
		_pf_Initialize();
	else
		std::cerr << "Intialize function not set!" << std::endl;
}

void Odorless::Engine::Base::Update(double deltaTime)
{
	if (_pf_Update != 0)
		_pf_Update(deltaTime);
	else
		std::cerr << "Update function not set!" << std::endl;
}

void Odorless::Engine::Base::Draw(double deltaTime)
{
	if (_pf_Draw != 0)
		_pf_Draw(deltaTime);
	else
		std::cerr << "Draw function not set!" << std::endl;
}

void Odorless::Engine::Base::Dispose()
{
	_bIsRunning = false;
	glfwDisable(GLFW_STICKY_KEYS);

	glfwTerminate();
	Odorless::Engine::Textures::TextureManager::Dispose();
}

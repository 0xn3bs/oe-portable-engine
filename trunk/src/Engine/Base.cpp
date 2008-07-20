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
#include "Input/Input.h"

int OE::Base::_iWindowWidth, OE::Base::_iWindowHeight = 0;
bool OE::Base::_bIsRunning = false;
void (*OE::Base::_pf_Initialize)(void) = 0;
void (*OE::Base::_pf_Update)(double deltaTime) = 0;
void (*OE::Base::_pf_Draw)(double deltaTime) = 0;

///	Function which Intializes the Engine. <b>Call this before anything else is done!</b>
unsigned int OE::Base::InitializeEngine(int argc, char **argv)
{
	glfwInit();
	/*
	ALuint buffer, source;
	alutInit (&argc, argv);
	buffer = alutCreateBufferFromFile("base/sounds/music/oe.wav");

	if(buffer == AL_NONE)
	{
		std::cout << "Failed to load sound" << std::endl;
	}

	alGenSources (1, &source);
	alSourcei (source, AL_BUFFER, buffer);
	alSourcePlay (source);
	*/
	_bIsRunning = true;
	OE::Input::InputManager::Initialize();
	OE::Textures::TextureManager::Initialize();
	return 0;
}

/*! \fn OEngine::Base::OpenWindow(int width, int height, int redbits, int greenbits, int bluebits, int alphabits, int depthbits, int stencilbits)
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
int OE::Base::OpenWindow(int width, int height, int redbits,
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

	_iWindowWidth = width;
	_iWindowHeight = height;

	if (!window)
		Dispose();

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: GLEW failed to load!" << std::endl;
	}

	OE::Input::InputManager::Initialize();

	return 0;
}

void OE::Base::Initialize()
{
	if (_pf_Initialize != 0)
		_pf_Initialize();
	else
		std::cerr << "Intialize function not set!" << std::endl;
}

void OE::Base::Update(double deltaTime)
{
	if (_pf_Update != 0)
		_pf_Update(deltaTime);
	else
		std::cerr << "Update function not set!" << std::endl;
}

void OE::Base::Draw(double deltaTime)
{
	if (_pf_Draw != 0)
		_pf_Draw(deltaTime);
	else
		std::cerr << "Draw function not set!" << std::endl;
}

void OE::Base::Dispose()
{
	_bIsRunning = false;
	//alutExit();
	glfwTerminate();
	OE::Textures::TextureManager::Dispose();
}

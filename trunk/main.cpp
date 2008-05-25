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
#include <windows.h>
#define __BUILD_STRING__ "Odorless Entertainment - Engine - Build: " __DATE__ " at " __TIME__
#include "Engine/Base.h"
#include "Engine/UI/Windows/WindowManager.h"
#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/Input/Input.h"
#include "Engine/Tools/Timers/Timer.h"
#include "Engine/Types/String.h"
#include "Game/UI/BasicWindow.h"
#include <iostream>

Odorless::Engine::Tools::Timers::Timer timer;
Odorless::Engine::Input::InputManager inputManager;
Odorless::Engine::UI::Windows::WindowManager windowManager;
Odorless::Game::UI::BasicWindow a(100, 100, 100, 100), b(200, 200, 150, 150);

void Initialize()
{
//	fontManager.AddFont("base/textures/fonts/arial", true);
//	fontManager.SetFont("base/textures/fonts/arial");

	windowManager.AddWindow(a);
	windowManager.AddWindow(b);
	windowManager.SetInput(&inputManager);

	glEnable(GL_LINE);
	glEnable(GL_POINT);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(0.0f, 1, 1, 0.0f, -1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	Odorless::Engine::Base::VSync(false);
}

void Update(double deltaTime)
{
	inputManager.Update();
	windowManager.Update(deltaTime);
}

void Draw(double deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(0, 1, 1, 1.0f);
	glViewport(0, 0, 800, 600);
	windowManager.Render(deltaTime);
}

void OnResize(int width, int height)
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport(0, 0, width, height);
	glOrtho(0.0f, 1, 1, 0.0f, -1.0f, 1.0f);
	windowManager.UpdateWin(width, height);
}

int main()
{
	Odorless::Engine::Base::InitializeEngine();
	Odorless::Engine::Base::OpenWindow(800, 600, 8, 8, 8, 8, 24, 8);
	windowManager.UpdateWin(800, 600);
	Odorless::Engine::Base::SetWindowResize(OnResize);
	Odorless::Engine::Base::SetWindowTitle(__BUILD_STRING__);
	Odorless::Engine::Base::SetInitialize(Initialize);
	Odorless::Engine::Base::SetUpdate(Update);
	Odorless::Engine::Base::SetDraw(Draw);

	Odorless::Engine::Base::Initialize();

	int totalFrames = 0;
	int FPS = 0;
	while (Odorless::Engine::Base::IsRunning())
	{
		float deltaTime = 0.0f;

		if(timer.IsRunning)
		{
			if(timer.GetElapsedMilliSec() >= 1000)
			{
				FPS = totalFrames;
				totalFrames = 0;
				timer.Start();
				char windowtitle[1024];
				sprintf(windowtitle, "%s :: FPS: %i", __BUILD_STRING__, FPS);
				Odorless::Engine::Base::SetWindowTitle(windowtitle);
			}
		}
		else
		{
			timer.Start();
		}

		Odorless::Engine::Base::Update(deltaTime);
		Odorless::Engine::Base::Draw(deltaTime);
		Odorless::Engine::Base::Flush();
		Odorless::Engine::Base::SwapBuffers();

		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			break;

		totalFrames++;
	}

	Odorless::Engine::Base::Dispose();

	return 0;
}

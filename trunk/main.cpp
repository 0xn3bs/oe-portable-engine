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
#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/Input/Input.h"
#include "Engine/Tools/Timers/Timer.h"
#include "Engine/Types/String.h"
#include "Game/UI/BasicWindow.h"
#include <iostream>

Odorless::Engine::UI::Fonts::FontManager fontManager;
Odorless::Engine::Tools::Timers::Timer timer;
Odorless::Engine::Input::InputManager inputManager;
Odorless::Game::UI::BasicWindow basicWindow(50, 50, 100, 100);

void Initialize()
{
	fontManager.AddFont("base/textures/fonts/arial", true);
	fontManager.SetFont("base/textures/fonts/arial");

	glEnable(GL_LINE);
	glEnable(GL_POINT);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);
}

void Update(double deltaTime)
{
	inputManager.Update();
}

void Draw(double deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	basicWindow.Render(deltaTime);
	glColor4f(1, 1, 1, 1.0f);
	glPushMatrix();
	Odorless::Engine::Types::String text("Gamedev.net\nNew line test!");
	text.Remove("\nNew");
	fontManager.Write(text.c_str());

	glPopMatrix();
}

void OnResize(int width, int height)
{
	Odorless::Engine::Base::VSync(false);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
}

int main()
{
	Odorless::Engine::Base::InitializeEngine();
	Odorless::Engine::Base::OpenWindow(800, 600, 8, 8, 8, 8, 24, 8);
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

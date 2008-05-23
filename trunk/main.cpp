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
#define _BUILD_STRING_ "Odorless Entertainment - Engine - Build: " __DATE__ " at " __TIME__
#include "Engine/Base.h"
#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/Tools/Timers/Timer.h"
#include <iostream>

Odorless::Engine::UI::Fonts::FontManager *fontManager;
Odorless::Engine::Tools::Timers::Timer *timer;
void Initialize()
{
	fontManager = new Odorless::Engine::UI::Fonts::FontManager();
	fontManager->AddFont("../../../base/textures/fonts/phantom");
	fontManager->SetFont("../../../base/textures/fonts/phantom");

	timer = new Odorless::Engine::Tools::Timers::Timer();

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
}

void Draw(double deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1, 1, 1, 1.0f);
	glPushMatrix();
	fontManager->Write("Gamedev.net\nNew line test!");
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
	Odorless::Engine::Base::SetWindowTitle(_BUILD_STRING_);
	Odorless::Engine::Base::SetInitialize(Initialize);
	Odorless::Engine::Base::SetUpdate(Update);
	Odorless::Engine::Base::SetDraw(Draw);

	Odorless::Engine::Base::Initialize();

	int totalFrames = 0;
	int FPS = 0;
	while (Odorless::Engine::Base::IsRunning())
	{
		float deltaTime = 0.0f;
		if(timer->IsRunning)
		{
			if(timer->GetElapsedMilliSec() >= 1000)
			{
				FPS = totalFrames;
				totalFrames = 0;
				timer->Start();
				char windowtitle[1024];
				sprintf(windowtitle, "%s :: FPS: %i", _BUILD_STRING_, FPS);
				Odorless::Engine::Base::SetWindowTitle(windowtitle);
			}
		}
		else
		{
			timer->Start();
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

	delete fontManager;

	return 0;
}

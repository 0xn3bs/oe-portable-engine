/*****************************************************************************************
* Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the GNU Lesser Public License v2.1
* which accompanies this distribution, and is available at
* http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
* 
* Contributors:
*     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
*	  Joshua 'blankthemuffin' Simmons - Comitted this line!
*****************************************************************************************/

#ifdef win32
#include <windows.h>
#endif

#define __BUILD_STRING__ "Odorless Entertainment - Engine - Build: " __DATE__ " at " __TIME__
#include "Engine/Base.h"
#include "Engine/Input/Input.h"
#include "Engine/Tools/Timers/Timer.h"
#include "Engine/Types/String.h"
#include "Engine/UI/Windows/WindowManager.h"
#include "Engine/UI/Widgets/Button.h"
#include "Engine/UI/Fonts/FontManager.h"
#include "Game/UI/BasicWindow.h"
#include "Engine/Parsers/INI.h"
#include "Engine/Parsers/BSP.h"
#include "Engine/Cameras/FPSCamera.h"
#include <iostream>
#include <math.h>

OEngine::Tools::Timers::Timer timerFPS;
OEngine::Tools::Timers::Timer timerAlpha;
//Odorless::Engine::UI::Windows::WindowManager windowManager;
//Odorless::Game::UI::BasicWindow a(150, 150, 100, 100), b(75, 75, 150, 150), c(20, 20, 50, 50);
OEngine::Parsers::INI iniParser;
OEngine::Parsers::BSP bspParser;
OEngine::Cameras::FPSCamera *fpsCamera;
bool IsGUIEnabled = false;
void Initialize()
{
	OEngine::Textures::TextureManager::LoadTexture("textures/notexture");
	//Odorless::Engine::UI::Fonts::FontManager::AddFont("base/textures/fonts/phantom", true);
	//Odorless::Engine::UI::Fonts::FontManager::SetFont("base/textures/fonts/phantom");

	//a.SetCaption("Window A");
	//b.SetCaption("Window B");
	//c.SetCaption("C");

	//windowManager.Initialize();
	//Odorless::Engine::UI::Widgets::Button *testButton = new Odorless::Engine::UI::Widgets::Button(20, 20, 100, 15.5, &b);
	//testButton->SetCaption("Button");
	//b.AddWidget(testButton);
	//windowManager.AddWindow(a);
	//windowManager.AddWindow(b);
	//windowManager.AddWindow(c);

	fpsCamera = new OEngine::Cameras::FPSCamera(0.0, 0.0, 10.0, 0.0, 0.0, -100.0, 0.0, 1.0, 0.0);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE);
	glEnable(GL_POINT);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//glEnable(GL_LIGHTING);
	glPointSize(2.0f);
	glLineWidth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	OEngine::Base::VSync(false);

	std::string mapToLoad = iniParser.GetString("default", "map");
	bspParser.ParseBSP(mapToLoad.c_str());
}
void Update(double deltaTime)
{
	OEngine::Input::InputManager::Update();
	if(IsGUIEnabled)
	{
		//windowManager.Update(deltaTime);
	}
	fpsCamera->Update(deltaTime);
}
void Draw(double deltaTime)
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColor4f(0, 1, 1, 1.0f);
	//glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	fpsCamera->Render();
	//gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, -100.0f, 0.0f, 1.0f, 0.0f);
	//glViewport(0, 0, 800, 600);
	//glBegin(GL_TRIANGLES);
	//	glVertex3f(10.0f, 0.0f, 1.0f);
	//	glVertex3f(0.0f, 10.0f, 1.0f);
	//	glVertex3f(-10.0f, 0.0f, 0.0f);
	//glEnd();
	bspParser.DebugRender();
	if(IsGUIEnabled)
	{
		//	Render GUI here
	}
	//glMatrixMode( GL_PROJECTION );
	//glViewport(0, 0, 800, 600);
	//windowManager.Render(deltaTime);
}
void OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(65, (GLfloat)width/(GLfloat)height, 0.1f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	OEngine::Input::InputManager::SetMouseReferencePos(width/2, height/2);
	OEngine::Input::InputManager::SetForceToMouseReference(true);
	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//glViewport(0, 0, width, height);
	//glOrtho(0.0f, 1, 1, 0.0f, -1.0f, 1.0f);
	//windowManager.UpdateWin(width, height);
}
int main()
{
	iniParser.ParseINI("base/cfg/default.ini");
	
	OEngine::Base::InitializeEngine();
	int iResWidth = iniParser.GetInt("video", "res_width");
	int iResHeight = iniParser.GetInt("video", "res_height");
	OEngine::Base::OpenWindow(iResWidth, iResHeight, 8, 8, 8, 8, 24, 8);
	OEngine::Base::SetWindowResize(OnResize);
	OEngine::Base::SetWindowTitle(__BUILD_STRING__);
	OEngine::Base::SetInitialize(Initialize);
	OEngine::Base::SetUpdate(Update);
	OEngine::Base::SetDraw(Draw);
	OEngine::Base::Initialize();

	int totalFrames = 0;
	int FPS = 0;
	float elapsedTime = 0.0f;
	float deltaTime = 0.0f;

	while (OEngine::Base::IsRunning())
	{
		if(timerAlpha.IsRunning)
		{
			float el = timerAlpha.GetElapsedSec();
			deltaTime = el - elapsedTime;
			elapsedTime = el;
		}
		else
			timerAlpha.Start();

		if(timerFPS.IsRunning)
		{
			if(timerFPS.GetElapsedMilliSec() >= 1000)
			{
				FPS = totalFrames;
				totalFrames = 0;
				timerFPS.Start();
				char windowtitle[1024];
				sprintf(windowtitle, "%s :: FPS: %i", __BUILD_STRING__, FPS);
				OEngine::Base::SetWindowTitle(windowtitle);
			}
		}
		else
		{
			timerFPS.Start();
		}

		OEngine::Base::Update(deltaTime);
		OEngine::Base::Draw(deltaTime);
		OEngine::Base::Flush();
		OEngine::Base::SwapBuffers();

		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			break;

		totalFrames++;
	}

	OEngine::Base::Dispose();

	return 0;
}

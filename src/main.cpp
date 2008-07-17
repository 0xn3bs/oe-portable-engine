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

#define __BUILD_STRING__ "Odorless Entertainment - Engine - Build: " __DATE__ " at " __TIME__
#include "Engine/Base.h"
#include "Engine/Input/Input.h"
#include "Engine/Tools/Timers/Timer.h"
#include "Engine/UI/Windows/WindowManager.h"
#include "Engine/UI/Widgets/Button.h"
#include "Engine/UI/Fonts/FontManager.h"
#include "Game/UI/BasicWindow.h"
#include "Engine/Parsers/INI.h"
#include "Engine/Parsers/BSP.h"
#include "Engine/Cameras/FPSCamera.h"
#include "Engine/Maths/Vector.h"
#include "Engine/Parsers/MD3.h"
#include <iostream>
#include <math.h>

OE::Tools::Timers::Timer timerFPS;
OE::Tools::Timers::Timer timerAlpha;
OE::Parsers::INI iniParser;
OE::Parsers::BSP *bspParser;
OE::Cameras::FPSCamera *fpsCamera;
OE::UI::Windows::WindowManager *windowManager;
OE::UI::Windows::Window *testWindow, *testWindow2;
OE::UI::Widgets::Button *testButton;
bool IsGUIEnabled = false;

void Initialize()
{
	OE::Textures::TextureManager::LoadTextureFromPath("base/textures/notexture.tga");
	OE::UI::Fonts::FontManager::AddFont("base/textures/fonts/phantom", true);
	OE::UI::Fonts::FontManager::SetFont("base/textures/fonts/phantom");

	fpsCamera = new OE::Cameras::FPSCamera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	fpsCamera->Update(0);
	windowManager = new OE::UI::Windows::WindowManager();
	testWindow = new OE::Game::UI::BasicWindow(100,100,100,100);
	testWindow2 = new OE::Game::UI::BasicWindow(100,100,250,125);
	testButton = new OE::UI::Widgets::Button(50, 50, 75, 10, testWindow2);
	testWindow2->AddWidget(testButton);
	windowManager->AddWindow(*testWindow);
	windowManager->AddWindow(*testWindow2);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	//glEnable(GL_ALPHA_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	//glEnable(GL_LIGHTING);
	glPointSize(2.0f);
	glLineWidth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	OE::Base::VSync(false);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	std::string mapToLoad = iniParser.GetString("default", "map");
	bspParser = new OE::Parsers::BSP();
	bspParser->ParseBSP(mapToLoad.c_str());

	OE::Maths::Vec3<float> cp;
	OE::Maths::Vec3<float> a(0.69f, 0.10f, 0.70f);
	OE::Maths::Vec3<float> b(0.44f, 0.71f, -0.54f);

	std::cout << a << " x " << b << " = " << a % b << std::endl;
}

void Update(double deltaTime)
{
	if(OE::Input::InputManager::IsCursorVisible())
	{
		windowManager->UpdateWin(OE::Base::_iWindowWidth, OE::Base::_iWindowHeight);
		windowManager->Update(deltaTime);
	}
	else
	{
			fpsCamera->Update(deltaTime);
	}
	OE::Input::InputManager::Update(deltaTime);
}

void Draw(double deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4ub(255,255,255,255);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(65, (GLfloat)OE::Base::_iWindowWidth/(GLfloat)OE::Base::_iWindowHeight, 1.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	fpsCamera->Render();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	bspParser->DebugRender(static_cast<float>(deltaTime), fpsCamera);
	glPopMatrix();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D,0);
	if(OE::Input::InputManager::IsCursorVisible())
	{
		windowManager->Render(deltaTime);
	}
	glViewport(0, 0, OE::Base::_iWindowWidth, OE::Base::_iWindowHeight);
}

void OnResize(int width, int height)
{
	OE::Base::_iWindowWidth = width;
	OE::Base::_iWindowHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65, (GLfloat)width/(GLfloat)height, 1.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	OE::Input::InputManager::SetMouseReferencePos(width/2, height/2);
	OE::Input::InputManager::SetForceToMouseReference(true);
}

int main(int argc, char **argv)
{
	iniParser.ParseINI("base/cfg/default.ini");
	
	OE::Base::InitializeEngine(argc, argv);
	int iResWidth = iniParser.GetInt("video", "res_width");
	int iResHeight = iniParser.GetInt("video", "res_height");
	OE::Base::OpenWindow(iResWidth, iResHeight, 8, 8, 8, 8, 24, 8);
	OE::Base::SetWindowResize(OnResize);
	OE::Base::SetWindowTitle(__BUILD_STRING__);
	OE::Base::SetInitialize(Initialize);
	OE::Base::SetUpdate(Update);
	OE::Base::SetDraw(Draw);
	OE::Base::Initialize();

	int totalFrames = 0;
	int FPS = 0;
	float elapsedTime = 0.0f;
	float deltaTime = 0.0f;

	while (OE::Base::IsRunning())
	{
		if(timerAlpha.IsRunning)
		{
			float el = static_cast<float>(timerAlpha.GetElapsedSec());
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
				OE::Base::SetWindowTitle(windowtitle);
			}
		}
		else
		{
			timerFPS.Start();
		}

		OE::Base::Update(deltaTime);
		OE::Base::Draw(deltaTime);
		OE::Base::Flush();
		OE::Base::SwapBuffers();

		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			break;

		totalFrames++;
	}

	OE::Base::Dispose();
	return 0;
}

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
bool IsGUIEnabled = false;

void Initialize()
{
	OE::Textures::TextureManager::LoadTextureFromPath("base/textures/notexture.tga");

	fpsCamera = new OE::Cameras::FPSCamera(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE);
	glEnable(GL_POINT);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	//glEnable(GL_ALPHA);
	//glEnable(GL_ALPHA_TEST);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR);
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
	OE::Input::InputManager::Update(deltaTime);
	if(IsGUIEnabled)
	{
		//windowManager.Update(deltaTime);
	}
	//fpsCamera->Update(deltaTime);
}

void Draw(double deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	fpsCamera->Render();
	bspParser->DebugRender(static_cast<float>(deltaTime), fpsCamera);
	if(IsGUIEnabled)
	{
		//	Render GUI here
	}
}

void OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode( GL_PROJECTION );
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

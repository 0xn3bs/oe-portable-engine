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
#ifndef BASE_H_
#define BASE_H_

#include <GL/glew.h>
#include <GL/glfw.h>
#include <AL/alut.h>

/*! \namespace Odorless
\brief The Odorless Entertainment's expandable namespace.
*/
namespace OE
{
	class Base
	{
	public:
		static unsigned int InitializeEngine(int argc, char **argv);
		static int OpenWindow(int width, int height, int redbits,
			int greenbits, int bluebits, int alphabits,
			int depthbits, int stencilbits);
		static void Initialize();
		static void Update(double deltaTime);
		static void Draw(double deltaTime);
		static void SetWindowTitle(const char* title)
		{
			glfwSetWindowTitle(title);
		}

		static void VSync(bool enabled)
		{
			if(enabled)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);
		}

		static void Dispose();
		static bool IsRunning()
		{
			return _bIsRunning;
		}

		static void SetInitialize(void (*initialize)(void))
		{
			_pf_Initialize = initialize;
		}

		static void SetUpdate(void (*update)(double deltaTime))
		{
			_pf_Update = update;
		}

		static void SetDraw(void (*draw)(double deltaTime))
		{
			_pf_Draw = draw;
		}

		static void GetWindowSize(int* width, int* height)
		{
			*width = _iWindowWidth;
			*height = _iWindowHeight;
		}

		static void SetWindowResize(void (*resize)(int width, int height))
		{
			glfwSetWindowSizeCallback((GLFWwindowsizefun)resize);
		}

		static void SwapBuffers()
		{
			glfwSwapBuffers();
		}

		static void Flush()
		{
			glFlush();
		}
	private:
		static int _iWindowWidth, _iWindowHeight;
		static bool _bIsRunning;
		static void (*_pf_Update)(double deltaTime);
		static void (*_pf_Draw)(double deltaTime);
		static void (*_pf_Initialize)(void);
	};
}

#endif /*BASE_H_*/

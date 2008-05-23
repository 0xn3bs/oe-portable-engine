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

#ifndef INPUT_H_
#define INPUT_H_

#include <GL/glfw.h>
#include <ostream>
#include <memory>

namespace Odorless
{
	namespace Engine
	{
		namespace Input
		{
			class Input
			{
				private:
					int _iMouseX, _iMouseY;
					bool _rgcKeys[255];
				public:
					Input()
					{
					}

					~Input()
					{
					}

					void SetKeyState(const char key, bool state);
					bool GetKeyState(const char key);

					void SetMousePos(const int x, const int y);
					void GetMousePos(int *pos);
					const int GetMouseX();
					const int GetMouseY();
			};
		};
	};
};
#endif /*INPUT_H_*/

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
#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include "Window.h"

#include <vector>

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Windows
			{
				class WindowManager
				{
					public:
						WindowManager();
						~WindowManager();
						void AddWindow(const Window &window);
						void RemoveWindow(const unsigned int &index);
						void Update(const float &dt);
						void Render(const float &dt);
					
					private:
						std::vector<Window*> _vecWindows;
				};
			}
		}
	}
}

#endif /*WINDOWMANAGER_H_*/

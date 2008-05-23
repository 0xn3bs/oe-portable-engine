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
#ifndef WIDGET_H_
#define WIDGET_H_

namespace Odorless
{
	namespace Engine
	{
		namespace UI
		{
			namespace Widgets
			{
				class Widget
				{
					public:
						Widget();
						virtual ~Widget();
						virtual void Initialize() = 0;
						virtual void Dispose() = 0;
						virtual void Update(const float &dt) = 0;
						virtual void Render(const float &dt) = 0;
						
					private:
						float _2fDimension[2];
						float _2fPosition[2];
						unsigned int _uiFgColor;
						unsigned int _uiBgColor;
				};
			}
		}
	}
}

#endif /*WIDGET_H_*/

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
#ifndef WINDOW_H_
#define WINDOW_H_

#include "Engine/GLFW/glfw.h"
#include "Engine/UI/Widgets/Widget.h"
#include "Engine/UI/Fonts/FontManager.h"
#include "Engine/Maths/Vector.h"
#include <vector>

namespace OE
{
	namespace UI
	{
		struct _THEME
		{
			unsigned char _4bBGColor[4];
			unsigned char _4bFGColor[4];
			unsigned char _4bTextColor[4];
			unsigned char _4bBorderColor[4];
		};

		namespace Widgets{class Widget;}
		namespace Windows
		{
			class WindowManager; //	Forward declaration for the Window Manager.
			class Window
			{
				friend class WindowManager;
			public:
				Window();
				Window(const float x, const float y, const float width, const float height);
				~Window();
				virtual int Initialize() = 0;
				virtual void Dispose() = 0;
				virtual void Update(const float dt) = 0;
				virtual void Render(const float dt) = 0;

				virtual void OnMouseClick(const char button, const int x, const int y){};
				virtual void OnMouseDown(const char button, const int x, const int y){};
				virtual void OnMouseUp(const char button, const int x, const int y){};
				virtual void OnMouseMove(int x, int y){};
				virtual void OnCharEvent(int key, int action){};

				int GetXPos(){return static_cast<int>(_v2fPosition.x);}
				int GetYPos(){return static_cast<int>(_v2fPosition.y);}
				int GetWidth(){return static_cast<int>(_v2fDimensions.x);}
				int GetHeight(){return static_cast<int>(_v2fDimensions.y);}

				bool IsOver(const float x, const float y);
				bool IsOverTitleBar(const float x, const float y);
				void AddWidget(UI::Widgets::Widget *widget);
				void RemoveWidget(const unsigned int index);
				void SetCaption(const char* caption)
				{
					_szCaption = caption;
				}

			protected:
				const char* _szCaption;

				OE::Maths::Vec2<float> _v2fDimensions;
				OE::Maths::Vec2<float> _v2fPosition;
				OE::Maths::Vec2<float> _v2fScale;

				float _fTitleBarY;

				unsigned char _uiFgColor[4];
				unsigned char _uiBgColor[4];
				unsigned char _uiBrdrColor[4];
				bool _bIsDragging;
				bool _bHasFocus;
				std::vector<UI::Widgets::Widget*> _vecWidgets;
			};
		}
	}
}

#endif /*WINDOW_H_*/

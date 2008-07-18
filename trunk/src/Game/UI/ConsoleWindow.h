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
#ifndef CONSOLE_WINDOW_H_
#define CONSOLE_WINDOW_H_

#include "Engine/UI/Windows/Window.h"
#include "Engine/Tools/Colors/Color.h"
#include "Engine/Textures/TextureManager.h"
#include "Engine/UI/Widgets/Button.h"
#include "Engine/UI/Widgets/TextField.h"

namespace OE
{
	namespace Game
	{
		namespace UI
		{
			class ConsoleWindow : public OE::UI::Windows::Window
			{
			private:
				OE::UI::Widgets::Widget* textField;
			public:
				ConsoleWindow() : OE::UI::Windows::Window()
				{
					
				}
				ConsoleWindow(const float x, const float y, const float width, const float height) : OE::UI::Windows::Window(x, y, width, height)
				{

				}
				~ConsoleWindow()
				{

				}
				int Initialize()
				{
					OE::Tools::Colors::Color::RGBA(_uiBgColor,36,97,104,190);
					OE::Tools::Colors::Color::RGBA(_uiBrdrColor,55,124,129,190);
					OE::Tools::Colors::Color::RGBA(_uiFgColor,236,225,195,190);
					textField = new OE::UI::Widgets::TextField(0, _v2fDimensions.y-20, _v2fDimensions.x,20, this);
					AddWidget(textField);
					_fTitleBarY = 15.5f;
					return 0;
				}
				void Dispose()
				{
				}
				void Update(const float dt)
				{

				}
				void Render(const float dt)
				{
					//	Background
					if(!_bHasFocus)
						glColor4ub(_uiBgColor[0],_uiBgColor[1],_uiBgColor[2],_uiBgColor[3]);
					else
						glColor4ub(_uiBgColor[2],_uiBgColor[1],_uiBgColor[0],_uiBgColor[3]);
					glBegin(GL_QUADS);
					glTexCoord2f(0, 0);
					glVertex3f(0, 0, 0);
					glTexCoord2f(0, 1);
					glVertex3f(0, 1, 0);
					glTexCoord2f(1, 1);
					glVertex3f(1, 1, 0);
					glTexCoord2f(1, 0);
					glVertex3f(1, 0, 0);
					glEnd();

					if(!_bHasFocus)
						glColor4ub(_uiBgColor[0]/2,_uiBgColor[1]/2,_uiBgColor[2]/2,_uiBgColor[3]);
					else
						glColor4ub(_uiBgColor[2]/2,_uiBgColor[1]/2,_uiBgColor[0]/2,_uiBgColor[3]);
				
					//	Render widgets here.
					for(unsigned int i = 0; i < _vecWidgets.size(); i++)
					{
						OE::UI::Widgets::Widget* wWidget = _vecWidgets.at(i);
						glTranslatef(wWidget->_v2fPosition.x/_v2fDimensions.x, wWidget->_v2fPosition.y/_v2fDimensions.y, 0);
						glScalef(wWidget->_v2fDimensions.x/_v2fDimensions.x,wWidget->_v2fDimensions.y/_v2fDimensions.y, 1);
						wWidget->Render(dt);
						glScalef(1/(wWidget->_v2fDimensions.x/_v2fDimensions.x), 1/(wWidget->_v2fDimensions.y/_v2fDimensions.y), 1);
						glTranslatef(-wWidget->_v2fPosition.x/_v2fDimensions.x, -wWidget->_v2fPosition.y/_v2fDimensions.y, 0);
					}

					//	Border
					glBegin(GL_LINES);
					glColor4ub(_uiBrdrColor[0],_uiBrdrColor[1],_uiBrdrColor[2],_uiBrdrColor[3]);
					glVertex3f(0, 0, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(0, 1, 0);
					glVertex3f(1, 1, 0);
					glVertex3f(1, 1, 0);
					glVertex3f(1, 0, 0);
					glVertex3f(1, 0, 0);
					glVertex3f(0, 0, 0);
					glVertex3f(0, _fTitleBarY/_v2fDimensions.y, 0);
					glVertex3f(1, _fTitleBarY/_v2fDimensions.y, 0);
					glEnd();

					glColor4ub(_uiFgColor[0],_uiFgColor[1],_uiFgColor[2],_uiFgColor[3]);
					glScalef(TEMP_FONT_SIZE/_v2fDimensions.x,TEMP_FONT_SIZE/_v2fDimensions.y, 1);
					OE::UI::Fonts::FontManager::Write("Console");
					glScalef(1/(TEMP_FONT_SIZE/_v2fDimensions.x), 1/(TEMP_FONT_SIZE/_v2fDimensions.y), 1);
				}
			};
		}
	}
}

#endif /*CONSOLE_WINDOW_H_*/

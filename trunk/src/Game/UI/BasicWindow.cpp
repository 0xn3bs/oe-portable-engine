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
#include "BasicWindow.h"

void OE::Game::UI::BasicWindow::Update(const float dt)
{
	for(unsigned int i = 0; i < _vecWidgets.size(); ++i)
	{
		OE::UI::Widgets::Widget* wWidget = _vecWidgets.at(i);
		wWidget->Update(dt);
	}
}

void OE::Game::UI::BasicWindow::Render(const float dt)
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
	
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1/_v2fDimensions.y, 0);
	glVertex3f(0, 1/_v2fDimensions.y, 0);
	glVertex3f(0, 0, 0);
	glEnd();

	//	Render widgets here.
	for(unsigned int i = 0; i < _vecWidgets.size(); ++i)
	{
		OE::UI::Widgets::Widget* wWidget = _vecWidgets.at(i);
		glTranslatef(wWidget->_v2fPosition.x/_v2fDimensions.x, wWidget->_v2fPosition.y/_v2fDimensions.y, 0);
		glScalef(wWidget->_v2fDimensions.x/_v2fDimensions.x,wWidget->_v2fDimensions.y/_v2fDimensions.y, 1);
		wWidget->Render(dt);
		glScalef(1/(wWidget->_v2fDimensions.x/_v2fDimensions.x), 1/(wWidget->_v2fDimensions.y/_v2fDimensions.y), 1);
		glTranslatef(-wWidget->_v2fPosition.x/_v2fDimensions.x, -wWidget->_v2fPosition.y/_v2fDimensions.y, 0);
	}

	//	Border
	glColor4ub(_uiBrdrColor[0],_uiBrdrColor[1],_uiBrdrColor[2],_uiBrdrColor[3]);
	glBegin(GL_LINES);
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

	//	Caption
	//glPushMatrix();
	glColor4ub(_uiFgColor[0],_uiFgColor[1],_uiFgColor[2],_uiFgColor[3]);
	//if(_szCaption == 0)
	glScalef(1.0f/_v2fDimensions.x,1.0f/_v2fDimensions.y,1);
		OE::UI::Fonts::FontManager::Write("Untitled");
	glScalef(1/(1.0f/_v2fDimensions.x),1/(1.0f/_v2fDimensions.y),1);
	//else
		//OE::UI::Fonts::FontManager::Write(_szCaption);
	//glPopMatrix();
}

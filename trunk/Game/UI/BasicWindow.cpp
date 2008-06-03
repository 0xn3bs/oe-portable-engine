#include "BasicWindow.h"

void Odorless::Game::UI::BasicWindow::Update(const float &dt)
{
	for(unsigned int i = 0; i < _vecWidgets.size(); i++)
	{
		Odorless::Engine::UI::Widgets::Widget* wWidget = _vecWidgets.at(i);
		wWidget->Update(dt);
	}
}

void Odorless::Game::UI::BasicWindow::Render(const float &dt)
{
	//	Background
	glBegin(GL_QUADS);
	if(!_bHasFocus)
		glColor4ub(_uiBgColor[0],_uiBgColor[1],_uiBgColor[2],_uiBgColor[3]);
	else
		glColor4ub(_uiBgColor[2],_uiBgColor[1],_uiBgColor[0],_uiBgColor[3]);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, _2fDimensions[1], 0);
	glTexCoord2f(1, 1);
	glVertex3f(_2fDimensions[0], _2fDimensions[1], 0);
	glTexCoord2f(1, 0);
	glVertex3f(_2fDimensions[0], 0, 0);
	if(!_bHasFocus)
		glColor4ub(_uiBgColor[0]/2,_uiBgColor[1]/2,_uiBgColor[2]/2,_uiBgColor[3]);
	else
		glColor4ub(_uiBgColor[2]/2,_uiBgColor[1]/2,_uiBgColor[0]/2,_uiBgColor[3]);
	glVertex3f(0, 0, 0);
	glVertex3f(0, _fTitleBarY, 0);
	glVertex3f(_2fDimensions[0], _fTitleBarY, 0);
	glVertex3f(_2fDimensions[0], 0, 0);
	glEnd();

	//	Render widgets here.
	for(unsigned int i = 0; i < _vecWidgets.size(); i++)
	{
		Odorless::Engine::UI::Widgets::Widget* wWidget = _vecWidgets.at(i);
		glTranslatef(wWidget->_2fPosition[0], wWidget->_2fPosition[1], 0);
		wWidget->Render(dt);
		glTranslatef(-wWidget->_2fPosition[0], -wWidget->_2fPosition[1], 0);
	}

	//	Border
	glBegin(GL_LINES);
	glColor4ub(_uiBrdrColor[0],_uiBrdrColor[1],_uiBrdrColor[2],_uiBrdrColor[3]);
	glVertex3f(0, 0, 0);
	glVertex3f(0, _2fDimensions[1], 0);
	glVertex3f(0, _2fDimensions[1], 0);
	glVertex3f(_2fDimensions[0], _2fDimensions[1], 0);
	glVertex3f(_2fDimensions[0], _2fDimensions[1], 0);
	glVertex3f(_2fDimensions[0], 0, 0);
	glVertex3f(_2fDimensions[0], 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, _fTitleBarY, 0);
	glVertex3f(_2fDimensions[0], _fTitleBarY, 0);
	glEnd();

	//	Caption
	glPushMatrix();
	glColor4ub(_uiFgColor[0],_uiFgColor[1],_uiFgColor[2],_uiFgColor[3]);
	if(_szCaption == 0)
		Engine::UI::Fonts::FontManager::Write("Untitled");
	else
		Engine::UI::Fonts::FontManager::Write(_szCaption);
	glPopMatrix();
}
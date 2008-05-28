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
#include "FontManager.h"
#include "Engine/Textures/TextureManager.h"
#include "Engine/Tools/Colors/Color.h"
#include <string>
#include <iostream>

Odorless::Engine::UI::Fonts::FontManager g_fontManager;

std::vector<Odorless::Engine::UI::Fonts::_FONT> Odorless::Engine::UI::Fonts::FontManager::_vFonts = std::vector<Odorless::Engine::UI::Fonts::_FONT>();
int Odorless::Engine::UI::Fonts::FontManager::_iCurrentFont = 0;

Odorless::Engine::UI::Fonts::FontManager::FontManager()
{
	_iCurrentFont = -1;
}

Odorless::Engine::UI::Fonts::FontManager::~FontManager()
{

}

//	TODO: Use alternative texture loading techniques as to not restrict to TGAs.
//	TODO: Generate non-monospace UV coordinates for characters.
const int Odorless::Engine::UI::Fonts::FontManager::AddFont(const char *path, bool setMonoSpaced)
{
	char szTGAPath[255];

	strcpy(szTGAPath, path);
	strcat(szTGAPath, ".tga");

	int uintTextureHandle = Odorless::Engine::Textures::TextureManager::LoadTexture(szTGAPath);

	if (uintTextureHandle <= -1)
		return uintTextureHandle;
	else
	{
		_FONT fontAddFont;

		fontAddFont._uintTextureHandle = uintTextureHandle;

		strcpy(fontAddFont._szName, path);

		CalculateUVs(fontAddFont, setMonoSpaced);
		_vFonts.push_back(fontAddFont);
		return uintTextureHandle;
	}

	return -1;
}

void Odorless::Engine::UI::Fonts::FontManager::CalculateUVs(_FONT &font, bool setMonoSpaced)
{
	if(setMonoSpaced)
	{
		const unsigned int cintFontSize = 256;
		const unsigned int cintFontCharSize = cintFontSize >> 4;

		for (int y = 0; y < cintFontSize; y+=cintFontCharSize)
		{
			for (int x = 0; x < cintFontSize; x+=cintFontCharSize)
			{
				float fU1, fU2, fV1, fV2;
				fU1 = (float)x / (float)cintFontSize;
				fU2 = (float)(x+cintFontCharSize) / (float)cintFontSize;
				fV1 = (float)y / (float)cintFontSize;
				fV2 = (float)(y+cintFontCharSize) / (float)cintFontSize;

				_CHARACTER characterChar;
				characterChar._fSize = cintFontCharSize;
				characterChar._fU1 = fU1;
				characterChar._fU2 = fU2;
				characterChar._fV1 = fV1;
				characterChar._fV2 = fV2;

				font._vChars.push_back(characterChar);
			}
		}
	}

	/*
	GLFWimage *image = new GLFWimage();
	char szTGAPath[255];

	strcpy(szTGAPath, font._szName);
	strcat(szTGAPath, ".tga");
	glfwReadImage( szTGAPath, image, GLFW_ORIGIN_UL_BIT );

	for( int r = image->Height-1; r >= 0; r-- )
	{
	for( int c = image->Width-1; c >= 0; c-- )
	{
	std::cout << c << "," << r << " : \t";

	int red = image->Data[r*c*image->BytesPerPixel];
	int g = image->Data[r*c*image->BytesPerPixel+1];
	int b = image->Data[r*c*image->BytesPerPixel+2];
	int a = image->Data[r*c*image->BytesPerPixel+3];

	std::cout << "r:" <<  red << "\t";
	std::cout << "g:" <<  g << "\t";
	std::cout << "b:" <<  b << "\t";
	std::cout << "a:" <<  a << std::endl;
	}
	}

	delete image;
	*/
}

void Odorless::Engine::UI::Fonts::FontManager::Write(const char* string)
{
	if (_iCurrentFont <= -1 || _iCurrentFont > (int)_vFonts.size())
	{
		std::cerr << "Error: Attempted to write text to screen with invalid font." << std::endl;
		return;
	}

	const unsigned int uintLength = (unsigned int)strlen(string);

	_FONT fFont = _vFonts.at(_iCurrentFont);

	glBindTexture(GL_TEXTURE_2D, fFont._uintTextureHandle);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	int iXPos = 0, iYPos = 0;
	for (unsigned int i = 0; i < uintLength; i++)
	{

		int intCharNum = (int)string[i];
		_CHARACTER character = fFont._vChars.at(intCharNum);

		if(string[i] == '\n')
		{
			iYPos += (int)character._fSize;
			iXPos = 0;
			continue;
		}

		glBegin(GL_QUADS);
		//Top-left vertex (corner)
		glTexCoord2f(character._fU1, character._fV1);
		glVertex3f((GLfloat)iXPos, (GLfloat)iYPos, 0);

		//Bottom-left vertex (corner)
		glTexCoord2f(character._fU1, character._fV2);
		glVertex3f((GLfloat)iXPos, (GLfloat)iYPos + character._fSize, 0);

		//Bottom-right vertex (corner)
		glTexCoord2f(character._fU2, character._fV2);
		glVertex3f((GLfloat)iXPos + character._fSize, (GLfloat)iYPos + character._fSize, 0);

		//Top-right vertex (corner)
		glTexCoord2f(character._fU2, character._fV1);
		glVertex3f((GLfloat)iXPos + character._fSize, (GLfloat)iYPos, 0);
		glEnd();

		iXPos += (int)character._fSize;
	}

	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Odorless::Engine::UI::Fonts::FontManager::SetFont(const char *name)
{
	if (IndexOf(name) > -1)
	{
		_iCurrentFont = IndexOf(name);
		std::clog << "Font set to '" << name << "'" << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
	}
}

void Odorless::Engine::UI::Fonts::FontManager::SetFont(const unsigned int &index)
{
	if (index < _vFonts.size() && index >= 0)
		_iCurrentFont = index;
}

int Odorless::Engine::UI::Fonts::FontManager::IndexOf(const char *name)
{
	for (unsigned int i = 0; i < _vFonts.size(); i++)
		if (strcmp(_vFonts[i]._szName, name) == 0)
			return i;

	return -1;
}

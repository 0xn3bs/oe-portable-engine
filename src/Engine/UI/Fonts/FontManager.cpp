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
#include "Engine/GLEW/glew.h"
#include "Engine/GL/glext.h"
#include "FontManager.h"
#include "Engine/Textures/TextureManager.h"
#include "Engine/Tools/Colors/Color.h"
#include <string>
#include <iostream>

OE::UI::Fonts::FontManager g_fontManager;

std::vector<OE::UI::Fonts::_FONT> OE::UI::Fonts::FontManager::_vFonts = std::vector<OE::UI::Fonts::_FONT>();
int OE::UI::Fonts::FontManager::_iCurrentFont = 0;

OE::UI::Fonts::FontManager::FontManager()
{
	_iCurrentFont = -1;
}

OE::UI::Fonts::FontManager::~FontManager()
{

}

//	TODO: Use alternative texture loading techniques as to not restrict to TGAs.
//	TODO: Generate non-monospace UV coordinates for characters.
const int OE::UI::Fonts::FontManager::AddFont(const char *path, bool setMonoSpaced)
{
	char szTGAPath[255];

	strcpy(szTGAPath, path);
	strcat(szTGAPath, ".png");

	int uintTextureHandle = OE::Textures::TextureManager::GetTexturesID(OE::Textures::TextureManager::LoadTextureFromPath(szTGAPath));

	if (uintTextureHandle <= -1)
		return uintTextureHandle;
	else
	{
		_FONT fontAddFont;

		fontAddFont.TextureHandle = uintTextureHandle;

		strcpy(fontAddFont.Name, path);

		CalculateUVs(fontAddFont, setMonoSpaced);
		_vFonts.push_back(fontAddFont);
		return uintTextureHandle;
	}

	return -1;
}

void OE::UI::Fonts::FontManager::CalculateUVs(_FONT &font, bool setMonoSpaced)
{	
	unsigned int iTexWidth = OE::Textures::TextureManager::GetTexturesWidth(font.TextureHandle-1);
	unsigned int iTexHeight = OE::Textures::TextureManager::GetTexturesHeight(font.TextureHandle-1);

	if(setMonoSpaced)
	{
		const unsigned int cintFontSize = 256;
		const unsigned int cintFontCharSize = cintFontSize >> 4;

		for (unsigned int y = 0; y < cintFontSize; y+=cintFontCharSize)
		{
			for (unsigned int x = 0; x < cintFontSize; x+=cintFontCharSize)
			{
				float fU1, fU2, fV1, fV2;
				fU1 = (float)x / (float)cintFontSize;
				fU2 = (float)(x+cintFontCharSize) / (float)cintFontSize;
				fV1 = (float)y / (float)cintFontSize;
				fV2 = (float)(y+cintFontCharSize) / (float)cintFontSize;

				_CHARACTER characterChar;
				characterChar.Size = cintFontCharSize;
				characterChar.U1 = fU1;
				characterChar.U2 = fU2;
				characterChar.V1 = fV1;
				characterChar.V2 = fV2;

				font.Chars.push_back(characterChar);
			}
		}
	}
}

void OE::UI::Fonts::FontManager::Write(const char* string)
{
	if (_iCurrentFont <= -1 || _iCurrentFont > (int)_vFonts.size())
	{
		std::cerr << "Error: Attempted to write text to screen with invalid font." << std::endl;
		return;
	}

	const unsigned int uintLength = (unsigned int)strlen(string);

	_FONT fFont = _vFonts.at(_iCurrentFont);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fFont.TextureHandle);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float iXPos = 0, iYPos = 0;

	glBegin(GL_QUADS);
	for (unsigned int i = 0; i < uintLength; ++i)
	{
		int intCharNum = (int)string[i];
		_CHARACTER character = fFont.Chars.at(intCharNum);
		if(string[i] == '\n')
		{
			iYPos += character.Size;
			iXPos = 0;
			continue;
		}

		
		//Top-left vertex (corner)
		glTexCoord2f(character.U1, character.V1);
		glVertex3f((GLfloat)iXPos, (GLfloat)iYPos, 0);

		//Bottom-left vertex (corner)
		glTexCoord2f(character.U1, character.V2);
		glVertex3f((GLfloat)iXPos, (GLfloat)iYPos + character.Size, 0);

		//Bottom-right vertex (corner)
		glTexCoord2f(character.U2, character.V2);
		glVertex3f((GLfloat)iXPos + character.Size, (GLfloat)iYPos + character.Size, 0);

		//Top-right vertex (corner)
		glTexCoord2f(character.U2, character.V1);
		glVertex3f((GLfloat)iXPos + character.Size, (GLfloat)iYPos, 0);
		

		iXPos += character.Size;
	}
	glEnd();

	glBlendFunc(GL_ZERO, GL_ZERO);
	glDisable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void OE::UI::Fonts::FontManager::SetFont(const char *name)
{
	if (IndexOf(name) > -1)
	{
		_iCurrentFont = IndexOf(name);
		std::clog << "Font set to '" << name << "'" << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
	}
}

void OE::UI::Fonts::FontManager::SetFont(const unsigned int index)
{
	if (index < _vFonts.size() && index >= 0)
		_iCurrentFont = index;
}

int OE::UI::Fonts::FontManager::IndexOf(const char *name)
{
	for (unsigned int i = 0; i < _vFonts.size(); ++i)
		if (strcmp(_vFonts[i].Name, name) == 0)
			return i;

	return -1;
}

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
#include "TextureManager.h"
#include <iostream>

std::vector<OE::Textures::_TEXTURE> OE::Textures::TextureManager::_vLoadedTextures = std::vector<OE::Textures::_TEXTURE>();
bool OE::Textures::TextureManager::_LoadImage(const char* path, GLuint Texture)
{
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(path, 0);
	FIBITMAP *imageFile = FreeImage_Load(fifmt, path, 0);

	if(imageFile == NULL)
		return false;

	unsigned int bpp = FreeImage_GetBPP(imageFile);
	GLint width = FreeImage_GetWidth(imageFile);
	GLint height = FreeImage_GetHeight(imageFile);

	_SwapRedAndBlueComponents(imageFile, width, height);
	FreeImage_FlipVertical(imageFile);

	int numLevels = _GenerateMipmaps(imageFile, width, height, bpp);

	std::clog << "Texture at " << "'" << path << "'" << " loaded as " 
		<< "texture #" << Texture << " :: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl;

	_TEXTURE temp;
	temp.TextureID = Texture;
	temp.Width = width;
	temp.Height = height;
	temp.BPP = bpp;
	temp.Levels = numLevels;

	_vLoadedTextures.push_back(temp);
	glBindTexture (GL_TEXTURE_2D, 0);
	FreeImage_Unload(imageFile);

	return true;
}

bool OE::Textures::TextureManager::_LoadRawImage(const unsigned char* data, GLuint Texture)
{
	FIBITMAP *imageFile = FreeImage_ConvertFromRawBits((BYTE*)data, 128, 128, 128*3, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);

	if(imageFile==NULL)
		return false;

	unsigned int bpp = FreeImage_GetBPP(imageFile);
	GLint width = FreeImage_GetWidth(imageFile);
	GLint height = FreeImage_GetHeight(imageFile);

	_SwapRedAndBlueComponents(imageFile, width, height);
	FreeImage_FlipVertical(imageFile);

	int numLevels = _GenerateMipmaps(imageFile, width, height, bpp);

	std::clog << "Raw texture loaded as texture #" << Texture << " :: " << __FILE__ << ":" << __LINE__ << std::endl;

	_TEXTURE temp;
	temp.TextureID = Texture;
	temp.Width = width;
	temp.Height = height;
	temp.BPP = bpp;
	temp.Levels = numLevels;

	_vLoadedTextures.push_back(temp);
	glBindTexture (GL_TEXTURE_2D, 0);
	FreeImage_Unload(imageFile);

	return true;
}

bool OE::Textures::TextureManager::_DoesFileExist(const char* path)
{
	FILE *fp;
	fp = fopen(path, "rb");
	if(fp == NULL)
		return false;

	fclose(fp);
	return true;
}

std::string OE::Textures::TextureManager::_GetTexturePath(const char* name)
{
	std::string tName = std::string(TEXTURE_PATH) + name;

	std::string tPath;

	//	Test extensions.
	tPath = tName + ".tga";
	if(_DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	tPath = tName + ".jpg";
	if(_DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	tPath = tName + ".jpeg";
	if(_DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	tPath = tName + ".png";
	if(_DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	return std::string("");
}

GLint OE::Textures::TextureManager::_LoadTextureFromPath(const char* path)
{
	std::string tPath = std::string(path);
	std::string ext = tPath.substr(tPath.find_last_of('.'), 4);

	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture (GL_TEXTURE_2D, Texture);

	if(_LoadImage(path, Texture))
	{
		return Texture;
	}

	//	We don't know what it is, free recently allocated texture memory.
	glBindTexture (GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &Texture);
	return -1;
}

GLint OE::Textures::TextureManager::_LoadTextureFromRaw(const unsigned char *data)
{
	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture (GL_TEXTURE_2D, Texture);

	if(_LoadRawImage(data, Texture))
	{
		return Texture;
	}

	//	We don't know what it is, free recently allocated texture memory.
	glBindTexture (GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &Texture);
	return -1;
}

int OE::Textures::TextureManager::LoadTextureFromPath(const char* name)
{
	std::string path = _GetTexturePath(name);
	GLint Texture = -1;

	if(path.length() == 0)
	{
		return -1;
	}

	if((Texture = _LoadTextureFromPath(path.c_str()))==-1)
	{
		std::cerr << "Error: Failed to load texture \"" << name << "\"" << std::endl;
		return -1;
	}

	return _vLoadedTextures.size()-1;
}

int OE::Textures::TextureManager::LoadTextureFromRaw(const unsigned char* data)
{
	if(data==NULL)
		return -1;

	GLint Texture = -1;

	if((Texture = _LoadTextureFromRaw(data)) == -1)
	{
		std::cerr << "Error: Failed to load RAW texture" << std::endl;
		return -1;
	}

	return _vLoadedTextures.size()-1;
}

void OE::Textures::TextureManager::DeleteTexture(const GLuint index)
{
	std::clog << "Texture #" << index << " deleted" << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
	glDeleteTextures(1, &index);
}

// This is quite hacky and messy..

void OE::Textures::TextureManager::Dispose()
{
	for( int i = 0; i < (int)_vLoadedTextures.size(); i++ )
	{
		DeleteTexture(_vLoadedTextures[i].TextureID);
	}

	_vLoadedTextures.empty();
}

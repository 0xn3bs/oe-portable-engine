#include "TextureManager.h"
#include <iostream>

GLint* OEngine::Textures::TextureManager::_vLoadedTextures = (GLint*)malloc(sizeof(GLint));
unsigned int OEngine::Textures::TextureManager::_iNumTextures = 0;

bool OEngine::Textures::TextureManager::LoadTGA(const char* path, GLuint Texture)
{
	if(glfwLoadTexture2D(path, GLFW_ORIGIN_UL_BIT))
	{
		std::clog << "Texture at " << "'" << path << "'" << " loaded as " 
			<< "texture #" << Texture << " :: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl;
		_iNumTextures++;
		_vLoadedTextures = (GLint*)realloc(_vLoadedTextures, sizeof(GLint) * (_iNumTextures + 1));
		_vLoadedTextures[_iNumTextures-1] = Texture;
		glBindTexture (GL_TEXTURE_2D, 0);
		return true;
	}
	glBindTexture (GL_TEXTURE_2D, 0);
	return false;
}

bool OEngine::Textures::TextureManager::LoadJPG(const char* path, GLuint Texture)
{
	FILE *fp;
	unsigned int fLength, width, height;
	unsigned char* buf;
	struct jdec_private *jpegDecoder;
	unsigned char* components[3];

	fp = fopen(path, "rb");
	if(fp==NULL)
		return false;
	fseek(fp, 0, SEEK_END);
	fLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (unsigned char *)malloc(fLength + 4);
	fread(buf, fLength, 1, fp);
	fclose(fp);

	jpegDecoder = tinyjpeg_init();

	if(jpegDecoder==NULL)
	{
		free(buf);
		return false;
	}

	if(tinyjpeg_parse_header(jpegDecoder, buf, fLength)<0)
	{
		free(buf);
		return false;
	}

	tinyjpeg_get_size(jpegDecoder, &width, &height);

	if(tinyjpeg_decode(jpegDecoder, TINYJPEG_FMT_RGB24)<0)
	{
		free(buf);
		return false;
	}

	tinyjpeg_get_components(jpegDecoder, components);

	int level = 0;
	while(width != 1 && height != 1)
	{
		glTexImage2D(GL_TEXTURE_2D, level, 3, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, components[0]);
		width /= 2;
		height /= 2;
		level++;
	}

	_iNumTextures++;
	_vLoadedTextures = (GLint*)realloc(_vLoadedTextures, sizeof(GLint)*(_iNumTextures+1));
	_vLoadedTextures[_iNumTextures-1] = Texture;

	glBindTexture (GL_TEXTURE_2D, 0);
	free(buf);
	return true;
}

bool OEngine::Textures::TextureManager::DoesFileExist(const char* path)
{
	FILE *fp;
	fp = fopen(path, "rb");
	if(fp == NULL)
		return false;

	fclose(fp);
	return true;
}

std::string OEngine::Textures::TextureManager::GetTexturePath(const char* name)
{
	std::string tName = std::string(TEXTURE_PATH) + name;

	std::string tPath;

	//	Test extensions.
	tPath = tName + ".jpg";
	if(DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	tPath = tName + ".tga";
	if(DoesFileExist(tPath.c_str()))
		return tPath.c_str();

	return std::string("");
}

GLint OEngine::Textures::TextureManager::_LoadTextureFromPath(const char* path)
{
	std::string tPath = std::string(path);
	std::string ext = tPath.substr(tPath.find_last_of('.'), 4);

	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture (GL_TEXTURE_2D, Texture);

	//	Is it a tga?
	if(ext.compare(".tga")==0)
	{
		if(LoadTGA(path, Texture))
		{
			return Texture;
		}
	}

	//	Is it a jpeg?
	if(ext.compare(".jpg")==0)
	{
		if(LoadJPG(path, Texture))
		{
			return Texture;
		}
	}

	//	We don't know what it is, free recently allocated texture memory.
	glBindTexture (GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &Texture);
	return -1;
}

GLint OEngine::Textures::TextureManager::LoadTexture(const char* name)
{
	std::string path = GetTexturePath(name);
	GLint Texture = -1;

	if(path.length() == 0)
	{
		return Texture;
	}

	if((Texture = _LoadTextureFromPath(path.c_str()))==-1)
	{
		std::cerr << "Error: Failed to load texture \"" << name << "\"" << std::endl;
	}

	return Texture;
}

void OEngine::Textures::TextureManager::DeleteTexture(const GLuint index)
{
	std::clog << "Texture #" << index << " freed from memory" << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
	glDeleteTextures(1, &index);
}

// This is quite hacky and messy..

void OEngine::Textures::TextureManager::Dispose()
{
	for( int i = 0; i < _iNumTextures; i++ )
	{
		DeleteTexture(_vLoadedTextures[i]);
	}

	free(_vLoadedTextures);
}

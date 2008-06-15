#include "TextureManager.h"
#include <iostream>

std::vector<GLint> Odorless::Engine::Textures::TextureManager::_vLoadedTextures = std::vector<GLint>();

bool Odorless::Engine::Textures::TextureManager::LoadTGA(const char *path, GLuint Texture)
{
	if(glfwLoadTexture2D(path, GLFW_ORIGIN_UL_BIT))
	{
		std::clog << "Texture at " << "'" << path << "'" << " loaded as " 
			<< "texture #" << Texture << " :: " << __FILE__ << ":" << __LINE__ << std::endl << std::endl;
		_vLoadedTextures.push_back(Texture);
		return true;
	}
	return false;
}

bool Odorless::Engine::Textures::TextureManager::LoadJPG(const char *path, GLuint Texture)
{
	_vLoadedTextures.push_back(Texture);
	return true;
	/*jpeg_decoder_file_stream jpegStream;
	if(!jpegStream.open(path))
		return false;

	int fileSize = jpegStream.get_size();
	size_t bytesRead;
	bool eof;
	unsigned char* fileBuf = new unsigned char[fileSize];
	bytesRead = jpegStream.read(fileBuf, fileSize, &eof);
	jpegStream.reset();

	return false;*/
}

GLint Odorless::Engine::Textures::TextureManager::LoadTexture(const char *path)
{
	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture (GL_TEXTURE_2D, Texture);

	const char* p = (path+(strlen(path)-4));

	if(strcmp(".tga", p)==0)
	{
		if(LoadTGA(path, Texture))
		{
			glBindTexture (GL_TEXTURE_2D, 0);
			return Texture;
		}
	}
	else
		if(strcmp(".jpg", p)==0)
		{
			if(LoadJPG(path, Texture))
			{
				glBindTexture (GL_TEXTURE_2D, 0);
				return Texture;
			}
		}


		/*
		if(glfwLoadTexture2D(path, GLFW_ORIGIN_UL_BIT))
		{
		std::clog << "Texture at " << "'" << path << "'" << " loaded as " 
		<< "texture #" << Texture << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
		_vLoadedTextures.push_back(Texture);
		return Texture;
		} 
		else
		{
		FILE *pFile;
		pFile = fopen(path, "rb");

		char* magic = (char*)malloc(sizeof(char)*5);
		fseek(pFile, 6, SEEK_SET);
		fread((char*)magic,sizeof(magic),1, pFile);
		magic[4] = '\0';
		fclose(pFile);

		if(strcmp(magic,"JFIF")==0)
		{

		}
		else
		{
		const GLubyte *errstring;
		errstring = gluErrorString(glGetError());
		glDeleteTextures(1, &Texture);
		return -1;
		}
		}
		*/
		glBindTexture (GL_TEXTURE_2D, 0);
		const GLubyte *errstring;
		errstring = gluErrorString(glGetError());
		glDeleteTextures(1, &Texture);
		return -1;
}

void Odorless::Engine::Textures::TextureManager::DeleteTexture(const GLuint index)
{
	std::clog << "Texture #" << index << " deleted" << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
	glDeleteTextures(1, &index);
}

// This is quite hacky and messy..

void Odorless::Engine::Textures::TextureManager::Dispose()
{
	for( int i = 0; i < (int)_vLoadedTextures.size(); i++ )
	{
		DeleteTexture(_vLoadedTextures[i]);
	}
}

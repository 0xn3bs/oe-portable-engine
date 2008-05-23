#include "TextureManager.h"
#include <iostream>

std::vector<GLint> Odorless::Engine::Textures::TextureManager::_vLoadedTextures = std::vector<GLint>();
GLint Odorless::Engine::Textures::TextureManager::LoadTexture(const char *path)
{
	GLuint Texture;
	glGenTextures(1, &Texture);
	glBindTexture (GL_TEXTURE_2D, Texture);
	
	if (glfwLoadTexture2D(path, GLFW_ORIGIN_UL_BIT))
	{
		std::clog << "Texture at " << "'" << path << "'" << " loaded as " 
		<< "texture #" << Texture << " :: " << __FILE__ << ":" << __LINE__ << std::endl;
		_vLoadedTextures.push_back(Texture);
		return Texture;
	} 
	else
	{
		const GLubyte *errstring;
		errstring = gluErrorString(glGetError());
		glDeleteTextures(1, &Texture);
		return -1;
	}
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
